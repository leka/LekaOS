/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_DMA/Src/decode_dma.c
  * @author  MCD Application Team
  * @brief   This file provides routines for JPEG decoding with DMA method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "decode_dma.h"
/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup JPEG_DecodingUsingFs_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t State;  
  uint8_t *DataBuffer;
  uint32_t DataBufferSize;

}JPEG_Data_BufferTypeDef;

/* Private define ------------------------------------------------------------*/

#define CHUNK_SIZE_IN  ((uint32_t)(4096)) 
#define CHUNK_SIZE_OUT ((uint32_t)(768))

#define JPEG_BUFFER_EMPTY 0
#define JPEG_BUFFER_FULL  1

#define NB_OUTPUT_DATA_BUFFERS      2
#define NB_INPUT_DATA_BUFFERS       2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
JPEG_YCbCrToRGB_Convert_Function pConvert_Function;
FIL *pFile;     /* pointer to File object */

uint8_t MCU_Data_OutBuffer0[CHUNK_SIZE_OUT];
uint8_t MCU_Data_OutBuffer1[CHUNK_SIZE_OUT];

uint8_t JPEG_Data_InBuffer0[CHUNK_SIZE_IN];
uint8_t JPEG_Data_InBuffer1[CHUNK_SIZE_IN];

JPEG_Data_BufferTypeDef Jpeg_OUT_BufferTab[NB_OUTPUT_DATA_BUFFERS] =
{
  {JPEG_BUFFER_EMPTY , MCU_Data_OutBuffer0 , 0},
  {JPEG_BUFFER_EMPTY , MCU_Data_OutBuffer1, 0}
};

JPEG_Data_BufferTypeDef Jpeg_IN_BufferTab[NB_INPUT_DATA_BUFFERS] =
{
  {JPEG_BUFFER_EMPTY , JPEG_Data_InBuffer0, 0},
  {JPEG_BUFFER_EMPTY , JPEG_Data_InBuffer1, 0}
};


uint32_t MCU_TotalNb = 0;

uint32_t MCU_BlockIndex = 0;
uint32_t Jpeg_HWDecodingEnd = 0;

uint32_t JPEG_OUT_Read_BufferIndex = 0;
uint32_t JPEG_OUT_Write_BufferIndex = 0;
__IO uint32_t Output_Is_Paused = 0;

uint32_t JPEG_IN_Read_BufferIndex = 0;
uint32_t JPEG_IN_Write_BufferIndex = 0;
__IO uint32_t Input_Is_Paused = 0;

uint32_t FrameBufferAddress;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FileName    : jpg file path for decode.
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress)
{
  uint32_t i;
  
  pFile = file;
  FrameBufferAddress = DestAddress;
          
  /* Read from JPG file and fill input buffers */
  for(i = 0; i < NB_INPUT_DATA_BUFFERS; i++)
  {
    if(f_read (pFile, Jpeg_IN_BufferTab[i].DataBuffer , CHUNK_SIZE_IN, (UINT*)(&Jpeg_IN_BufferTab[i].DataBufferSize)) == FR_OK)
    {
      Jpeg_IN_BufferTab[i].State = JPEG_BUFFER_FULL;
    }
    else
    {
      OnError_Handler();
    }        
  } 
  /* Start JPEG decoding with DMA method */
  HAL_JPEG_Decode_DMA(hjpeg ,Jpeg_IN_BufferTab[0].DataBuffer ,Jpeg_IN_BufferTab[0].DataBufferSize ,Jpeg_OUT_BufferTab[0].DataBuffer ,CHUNK_SIZE_OUT);
  
  return 0;
}

/**
  * @brief  JPEG Ouput Data BackGround Postprocessing .
  * @param hjpeg: JPEG handle pointer
  * @retval 1 : if JPEG processing has finiched, 0 : if JPEG processing still ongoing
  */
uint32_t JPEG_OutputHandler(JPEG_HandleTypeDef *hjpeg)
{
  uint32_t ConvertedDataCount;
  
  if(Jpeg_OUT_BufferTab[JPEG_OUT_Read_BufferIndex].State == JPEG_BUFFER_FULL)
  {  
    MCU_BlockIndex += pConvert_Function(Jpeg_OUT_BufferTab[JPEG_OUT_Read_BufferIndex].DataBuffer, (uint8_t *)FrameBufferAddress, MCU_BlockIndex, Jpeg_OUT_BufferTab[JPEG_OUT_Read_BufferIndex].DataBufferSize, &ConvertedDataCount);   
    
    Jpeg_OUT_BufferTab[JPEG_OUT_Read_BufferIndex].State = JPEG_BUFFER_EMPTY;
    Jpeg_OUT_BufferTab[JPEG_OUT_Read_BufferIndex].DataBufferSize = 0;
    
    JPEG_OUT_Read_BufferIndex++;
    if(JPEG_OUT_Read_BufferIndex >= NB_OUTPUT_DATA_BUFFERS)
    {
      JPEG_OUT_Read_BufferIndex = 0;
    }
    
    if(MCU_BlockIndex == MCU_TotalNb)
    {
      return 1;
    }
  }
  else if((Output_Is_Paused == 1) && \
          (Jpeg_OUT_BufferTab[JPEG_OUT_Write_BufferIndex].State == JPEG_BUFFER_EMPTY) &&\
          (Jpeg_OUT_BufferTab[JPEG_OUT_Read_BufferIndex].State == JPEG_BUFFER_EMPTY))
  {
    Output_Is_Paused = 0;
    HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);            
  }

  return 0;  
}

/**
  * @brief  JPEG Input Data BackGround processing .
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void JPEG_InputHandler(JPEG_HandleTypeDef *hjpeg)
{
  if(Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].State == JPEG_BUFFER_EMPTY)
  {
    if(f_read (pFile, Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].DataBuffer , CHUNK_SIZE_IN, (UINT*)(&Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].DataBufferSize)) == FR_OK)
    {  
      Jpeg_IN_BufferTab[JPEG_IN_Write_BufferIndex].State = JPEG_BUFFER_FULL;
    }
    else
    {
      OnError_Handler();
    }
    
    if((Input_Is_Paused == 1) && (JPEG_IN_Write_BufferIndex == JPEG_IN_Read_BufferIndex))
    {
      Input_Is_Paused = 0;
      HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBuffer, Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize);    
  
      HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT); 
    }
    
    JPEG_IN_Write_BufferIndex++;
    if(JPEG_IN_Write_BufferIndex >= NB_INPUT_DATA_BUFFERS)
    {
      JPEG_IN_Write_BufferIndex = 0;
    }            
  }
}

/**
  * @brief  JPEG Info ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pInfo: JPEG Info Struct pointer
  * @retval None
  */
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
  if(pInfo->ChromaSubsampling == JPEG_420_SUBSAMPLING)
  {
    if((pInfo->ImageWidth % 16) != 0)
    pInfo->ImageWidth += (16 - (pInfo->ImageWidth % 16));

    if((pInfo->ImageHeight % 16) != 0)
    pInfo->ImageHeight += (16 - (pInfo->ImageHeight % 16));
  }

  if(pInfo->ChromaSubsampling == JPEG_422_SUBSAMPLING)
  {
    if((pInfo->ImageWidth % 16) != 0)
    pInfo->ImageWidth += (16 - (pInfo->ImageWidth % 16));

    if((pInfo->ImageHeight % 8) != 0)
    pInfo->ImageHeight += (8 - (pInfo->ImageHeight % 8));
  }

  if(pInfo->ChromaSubsampling == JPEG_444_SUBSAMPLING)
  {
    if((pInfo->ImageWidth % 8) != 0)
    pInfo->ImageWidth += (8 - (pInfo->ImageWidth % 8));

    if((pInfo->ImageHeight % 8) != 0)
    pInfo->ImageHeight += (8 - (pInfo->ImageHeight % 8));
  }

  if(JPEG_GetDecodeColorConvertFunc(pInfo, &pConvert_Function, &MCU_TotalNb) != HAL_OK)
  {
    OnError_Handler();
  }
}

/**
  * @brief  JPEG Get Data callback
  * @param hjpeg: JPEG handle pointer
  * @param NbDecodedData: Number of decoded (consummed) bytes from input buffer
  * @retval None
  */
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
  if(NbDecodedData == Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize)
  {  
    Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].State = JPEG_BUFFER_EMPTY;
    Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize = 0;
  
    JPEG_IN_Read_BufferIndex++;
    if(JPEG_IN_Read_BufferIndex >= NB_INPUT_DATA_BUFFERS)
    {
      JPEG_IN_Read_BufferIndex = 0;        
    }
  
    if(Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].State == JPEG_BUFFER_EMPTY)
    {
      HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
      Input_Is_Paused = 1;
    }
    else
    {    
      HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBuffer, Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize);    
    }
  }
  else
  {
    HAL_JPEG_ConfigInputBuffer(hjpeg,Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBuffer + NbDecodedData, Jpeg_IN_BufferTab[JPEG_IN_Read_BufferIndex].DataBufferSize - NbDecodedData);      
  }
}

/**
  * @brief  JPEG Data Ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pDataOut: pointer to the output data buffer
  * @param OutDataLength: length of output buffer in bytes
  * @retval None
  */
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
  Jpeg_OUT_BufferTab[JPEG_OUT_Write_BufferIndex].State = JPEG_BUFFER_FULL;
  Jpeg_OUT_BufferTab[JPEG_OUT_Write_BufferIndex].DataBufferSize = OutDataLength;
    
  JPEG_OUT_Write_BufferIndex++;
  if(JPEG_OUT_Write_BufferIndex >= NB_OUTPUT_DATA_BUFFERS)
  {
    JPEG_OUT_Write_BufferIndex = 0;        
  }

  if(Jpeg_OUT_BufferTab[JPEG_OUT_Write_BufferIndex].State != JPEG_BUFFER_EMPTY)
  {
    HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
    Output_Is_Paused = 1;
  }
  HAL_JPEG_ConfigOutputBuffer(hjpeg, Jpeg_OUT_BufferTab[JPEG_OUT_Write_BufferIndex].DataBuffer, CHUNK_SIZE_OUT); 
}

/**
  * @brief  JPEG Error callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
  OnError_Handler();
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
  Jpeg_HWDecodingEnd = 1; 
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
