// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#define USE_DECODE_DMA	   0
#define USE_DECODE_POLLING 1

#define LCD_FRAME_BUFFER		0xC0000000
#define JPEG_OUTPUT_DATA_BUFFER 0xC0200000

#include "FATFileSystem.h"
#include "jpeg_utils.h"

#if USE_DECODE_DMA
	#include "decode_dma.h"
#elif USE_DECODE_POLLING
	#include "decode_polling.h"
#endif

extern JPEG_HandleTypeDef hjpeg;
extern JPEG_ConfTypeDef hjpeginfo;

namespace leka {
void JPEGMspInit();
void JPEGMspDeInit();
void JPEGInit();
}	// namespace leka

#if USE_DECODE_DMA
void JPEG_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);
#endif

// void OnError_Handler(const char *file, int line);
void OnError_Handler();

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
