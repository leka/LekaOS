// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "decode.h"

JPEG_HandleTypeDef hjpeg;

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

}	// namespace leka

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