/**
 ******************************************************************************
 * @file    jpeg_utils.h
 * @author  MCD Application Team
 * @version V2.0.0
 * @date    3-June-2016
 * @brief   Header for jpeg_utils.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __JPEG_UTILS_H
	#define __JPEG_UTILS_H

/* Includes ------------------------------------------------------------------*/
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_jpeg.h"
	#include "internal/corevideo_config.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __JPEG_MCU_RGB_ConvertorTypeDef {
	uint32_t ColorSpace;
	uint32_t ChromaSubsampling;

	uint32_t ImageWidth;
	uint32_t ImageHeight;
	uint32_t ImageSize_Bytes;

	uint32_t LineOffset;
	uint32_t BlockSize;

	uint32_t H_factor;
	uint32_t V_factor;

	uint32_t WidthExtend;
	uint32_t ScaledWidth;

	uint32_t MCU_Total_Nb;

	uint16_t *Y_MCU_LUT;
	uint16_t *Cb_MCU_LUT;
	uint16_t *Cr_MCU_LUT;
	uint16_t *K_MCU_LUT;

} JPEG_MCU_RGB_ConvertorTypeDef;

	/* Private define ------------------------------------------------------------*/
	#define YCBCR_420_BLOCK_SIZE 384   // YCbCr 4:2:0 MCU : 4 8x8 blocks of Y + 1 8x8 block of Cb + 1 8x8 block of Cr
	#define YCBCR_422_BLOCK_SIZE 256   // YCbCr 4:2:2 MCU : 2 8x8 blocks of Y + 1 8x8 block of Cb + 1 8x8 block of Cr
	#define YCBCR_444_BLOCK_SIZE 192   // YCbCr 4:4:4 MCU : 1 8x8 block of Y + 1 8x8 block of Cb + 1 8x8 block of Cr

	#define GRAY_444_BLOCK_SIZE 64	 // GrayScale MCU : 1 8x8 block of Y

	#define CMYK_444_BLOCK_SIZE                                                                                        \
		256	  // CMYK MCU : 1 8x8 blocks of Cyan + 1 8x8 block Magenta + 1 8x8 block of Yellow and 1 8x8 block of BlacK

	#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
		#define JPEG_GREEN_OFFSET	 8	  // Offset of the GREEN color in a pixel
		#define JPEG_ALPHA_OFFSET	 24	  // Offset of the Transparency Alpha in a pixel
		#define JPEG_BYTES_PER_PIXEL 4	  // Number of bytes in a pixel
		#if (JPEG_SWAP_RB == 0)
			#define JPEG_RED_OFFSET	 16	  // Offset of the RED color in a pixel
			#define JPEG_BLUE_OFFSET 0	  // Offset of the BLUE color in a pixel
		#else
			#define JPEG_RED_OFFSET	 0	  // Offset of the RED color in a pixel
			#define JPEG_BLUE_OFFSET 16	  // Offset of the BLUE color in a pixel
		#endif							  /* JPEG_SWAP_RB */

	#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
		#define JPEG_GREEN_OFFSET	 8	 // Offset of the GREEN color in a pixel
		#define JPEG_BYTES_PER_PIXEL 3	 // Number of bytes in a pixel

		#if (JPEG_SWAP_RB == 0)
			#define JPEG_RED_OFFSET	 16	  // Offset of the RED color in a pixel
			#define JPEG_BLUE_OFFSET 0	  // Offset of the BLUE color in a pixel
		#else
			#define JPEG_RED_OFFSET	 0	  // Offset of the RED color in a pixel
			#define JPEG_BLUE_OFFSET 16	  // Offset of the BLUE color in a pixel
		#endif							  /* JPEG_SWAP_RB  */

	#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
		#define JPEG_GREEN_OFFSET	   5		// Offset of the GREEN color in a pixel
		#define JPEG_BYTES_PER_PIXEL   2		// Number of bytes in a pixel
		#define JPEG_RGB565_GREEN_MASK 0x07E0	// Mask of Green component in RGB565 Format

		#if (JPEG_SWAP_RB == 0)
			#define JPEG_RED_OFFSET		  11	   // Offset of the RED color in a pixel
			#define JPEG_BLUE_OFFSET	  0		   // Offset of the BLUE color in a pixel
			#define JPEG_RGB565_RED_MASK  0xF800   // Mask of Red component in RGB565 Format
			#define JPEG_RGB565_BLUE_MASK 0x001F   // Mask of Blue component in RGB565 Format
		#else
			#define JPEG_RED_OFFSET		  0		   // Offset of the RED color in a pixel
			#define JPEG_BLUE_OFFSET	  11	   // Offset of the BLUE color in a pixel
			#define JPEG_RGB565_RED_MASK  0x001F   // Mask of Red component in RGB565 Format
			#define JPEG_RGB565_BLUE_MASK 0xF800   // Mask of Blue component in RGB565 Format
		#endif									   /* JPEG_SWAP_RB */

	#else

		#error "unknown JPEG_RGB_FORMAT "

	#endif /* JPEG_RGB_FORMAT */

/* Private macro -------------------------------------------------------------*/

	#define CLAMP(value) CLAMP_LUT[(value) + 0x100]	  // Range limitting macro

/* Public function prototypes -----------------------------------------------*/

using JPEG_YCbCrToRGB_Convert_Function = uint32_t (*)(uint8_t *pInBuffer, uint8_t *pOutBuffer, uint32_t BlockIndex,
													  uint32_t DataCount, uint32_t *ConvertedDataCount);

void JPEG_InitColorTables(void);

HAL_StatusTypeDef JPEG_GetDecodeColorConvertFunc(JPEG_ConfTypeDef *pJpegInfo,
												 JPEG_YCbCrToRGB_Convert_Function *pFunction, uint32_t *ImageNbMCUs);

/* Private function prototypes -----------------------------------------------*/

uint32_t JPEG_MCU_YCbCr420_ARGB_ConvertBlocks(uint8_t *pInBuffer, uint8_t *pOutBuffer, uint32_t BlockIndex,
											  uint32_t DataCount, uint32_t *ConvertedDataCount);

uint32_t JPEG_MCU_YCbCr422_ARGB_ConvertBlocks(uint8_t *pInBuffer, uint8_t *pOutBuffer, uint32_t BlockIndex,
											  uint32_t DataCount, uint32_t *ConvertedDataCount);

uint32_t JPEG_MCU_YCbCr444_ARGB_ConvertBlocks(uint8_t *pInBuffer, uint8_t *pOutBuffer, uint32_t BlockIndex,
											  uint32_t DataCount, uint32_t *ConvertedDataCount);

uint32_t JPEG_MCU_Gray_ARGB_ConvertBlocks(uint8_t *pInBuffer, uint8_t *pOutBuffer, uint32_t BlockIndex,
										  uint32_t DataCount, uint32_t *ConvertedDataCount);

uint32_t JPEG_MCU_YCCK_ARGB_ConvertBlocks(uint8_t *pInBuffer, uint8_t *pOutBuffer, uint32_t BlockIndex,
										  uint32_t DataCount, uint32_t *ConvertedDataCount);
void JPEG_InitPostProcColorTables(void);

#endif /* __JPEG_UTILS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
