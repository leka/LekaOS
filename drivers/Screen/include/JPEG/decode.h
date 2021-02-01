// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#define USE_DECODE_DMA		 0
#define USE_DECODE_POLLING	 1
#define USE_DECODE_INTERRUPT 0

#define JPEG_OUTPUT_DATA_BUFFER 0xC0200000

#include "FATFileSystem.h"
#include "Screen.h"
#include "dma2d.h"
#include "jpeg_utils.h"

#if USE_DECODE_DMA
	#include "decode_dma.h"
#elif USE_DECODE_POLLING
	#include "decode_polling.h"
#elif USE_DECODE_INTERRUPT
	#include "decode_interrupt.h"
#endif

namespace leka {
namespace jpeg {
	void mspInit();
	void mspDeInit();
	void init();

	void display(FIL *JPEG_File);
}	// namespace jpeg
}	// namespace leka

#if USE_DECODE_INTERRUPT
void JPEG_IRQHandler(void);
#endif
#if USE_DECODE_DMA
void JPEG_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);
#endif

// void OnError_Handler(const char *file, int line);
void OnError_Handler();
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg);

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
