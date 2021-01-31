// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "decode.h"

JPEG_HandleTypeDef hjpeg;
JPEG_ConfTypeDef hjpeginfo;
#if USE_DECODE_DMA
uint32_t JpegProcessing_End = 0;
#endif

namespace leka {

void JPEGMspInit()
{
	__HAL_RCC_JPEG_CLK_ENABLE();

#if USE_DECODE_DMA
	static DMA_HandleTypeDef hdmaIn;
	static DMA_HandleTypeDef hdmaOut;

	/* Enable JPEG clock */
	// __HAL_RCC_JPEG_CLK_ENABLE();

	/* Enable DMA clock */
	__HAL_RCC_DMA2_CLK_ENABLE();

	HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
	HAL_NVIC_EnableIRQ(JPEG_IRQn);

	/* Input DMA */
	/* Set the parameters to be configured */
	hdmaIn.Init.Channel				= DMA_CHANNEL_9;
	hdmaIn.Init.Direction			= DMA_MEMORY_TO_PERIPH;
	hdmaIn.Init.PeriphInc			= DMA_PINC_DISABLE;
	hdmaIn.Init.MemInc				= DMA_MINC_ENABLE;
	hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdmaIn.Init.MemDataAlignment	= DMA_MDATAALIGN_WORD;
	hdmaIn.Init.Mode				= DMA_NORMAL;
	hdmaIn.Init.Priority			= DMA_PRIORITY_HIGH;
	hdmaIn.Init.FIFOMode			= DMA_FIFOMODE_ENABLE;
	hdmaIn.Init.FIFOThreshold		= DMA_FIFO_THRESHOLD_FULL;
	hdmaIn.Init.MemBurst			= DMA_MBURST_INC4;
	hdmaIn.Init.PeriphBurst			= DMA_PBURST_INC4;

	hdmaIn.Instance = DMA2_Stream3;

	/* Associate the DMA handle */
	__HAL_LINKDMA(&hjpeg, hdmain, hdmaIn);

	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

	/* DeInitialize the DMA Stream */
	HAL_DMA_DeInit(&hdmaIn);
	/* Initialize the DMA stream */
	HAL_DMA_Init(&hdmaIn);

	/* Output DMA */
	/* Set the parameters to be configured */
	hdmaOut.Init.Channel			 = DMA_CHANNEL_9;
	hdmaOut.Init.Direction			 = DMA_PERIPH_TO_MEMORY;
	hdmaOut.Init.PeriphInc			 = DMA_PINC_DISABLE;
	hdmaOut.Init.MemInc				 = DMA_MINC_ENABLE;
	hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdmaOut.Init.MemDataAlignment	 = DMA_MDATAALIGN_WORD;
	hdmaOut.Init.Mode				 = DMA_NORMAL;
	hdmaOut.Init.Priority			 = DMA_PRIORITY_VERY_HIGH;
	hdmaOut.Init.FIFOMode			 = DMA_FIFOMODE_ENABLE;
	hdmaOut.Init.FIFOThreshold		 = DMA_FIFO_THRESHOLD_FULL;
	hdmaOut.Init.MemBurst			 = DMA_MBURST_INC4;
	hdmaOut.Init.PeriphBurst		 = DMA_PBURST_INC4;

	hdmaOut.Instance = DMA2_Stream4;
	/* DeInitialize the DMA Stream */
	HAL_DMA_DeInit(&hdmaOut);
	/* Initialize the DMA stream */
	HAL_DMA_Init(&hdmaOut);

	/* Associate the DMA handle */
	__HAL_LINKDMA(&hjpeg, hdmaout, hdmaOut);

	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
#endif
}

void JPEGMspDeInit()
{
#if USE_DECODE_DMA
	HAL_NVIC_DisableIRQ(DMA2_Stream4_IRQn);

	/* DeInitialize the MDMA Stream */
	HAL_DMA_DeInit(hjpeg.hdmain);

	/* DeInitialize the MDMA Stream */
	HAL_DMA_DeInit(hjpeg.hdmaout);
#endif
}

void JPEGInit()
{
	JPEG_InitColorTables();

	hjpeg.Instance = JPEG;
	// HAL_JPEG_MspInit(&hjpeg);
	HAL_JPEG_Init(&hjpeg);
}

void displayImage(FIL *JPEG_File)
{
	uint32_t width_offset  = 0;
	uint16_t screen_width  = 800;
	uint16_t screen_height = 480;

	jpeg_decode(&hjpeg, JPEG_File, JPEG_OUTPUT_DATA_BUFFER);

	HAL_JPEG_GetInfo(&hjpeg, &hjpeginfo);

	uint16_t xPos = (screen_width - hjpeginfo.ImageWidth) / 2;
	uint16_t yPos = (screen_height - hjpeginfo.ImageHeight) / 2;
	if (hjpeginfo.ChromaSubsampling == JPEG_420_SUBSAMPLING) {
		if ((hjpeginfo.ImageWidth % 16) != 0) width_offset = 16 - (hjpeginfo.ImageWidth % 16);
	}

	if (hjpeginfo.ChromaSubsampling == JPEG_422_SUBSAMPLING) {
		if ((hjpeginfo.ImageWidth % 16) != 0) width_offset = 16 - (hjpeginfo.ImageWidth % 16);
	}

	if (hjpeginfo.ChromaSubsampling == JPEG_444_SUBSAMPLING) {
		if ((hjpeginfo.ImageWidth % 8) != 0) width_offset = (hjpeginfo.ImageWidth % 8);
	}

	leka::DMA2DImage((uint32_t *)JPEG_OUTPUT_DATA_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, xPos, yPos,
					 hjpeginfo.ImageWidth, hjpeginfo.ImageHeight, width_offset);
}

void jpeg_decode(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress)
{
#if USE_DECODE_DMA
	JPEG_Decode_DMA(hjpeg, file, DestAddress);

	do {
		JPEG_InputHandler(hjpeg);
		JpegProcessing_End = JPEG_OutputHandler(hjpeg);

	} while (JpegProcessing_End == 0);

#elif USE_DECODE_POLLING
	JPEG_DecodePolling(hjpeg, file, DestAddress);
#endif
}

}	// namespace leka

#if USE_DECODE_DMA
void JPEG_IRQHandler(void)
{
	HAL_JPEG_IRQHandler(&hjpeg);
}

void DMA2_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(hjpeg.hdmain);
}

void DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(hjpeg.hdmaout);
}
#endif

// /**
//  * @brief  On Error Handler.
//  * @param  None
//  * @retval None
//  */
// void OnError_Handler(const char *file, int line)
// {
// 	printf("Error crash in %s line %d\n\r", file, line);
// 	while (1) {
// 		;
// 	} /* Blocking on error */
// }

/**
 * @brief  On Error Handler.
 * @param  None
 * @retval None
 */
void OnError_Handler()
{
	printf("Error crash\n\r");
	while (1) {
		;
	} /* Blocking on error */
}