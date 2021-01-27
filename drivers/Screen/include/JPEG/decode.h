// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#define USE_DECODE_DMA	   0
#define USE_DECODE_POLLING 1

#include "FATFileSystem.h"
#include "jpeg_utils.h"

#if USE_DECODE_DMA
	#include "decode_dma.h"
#elif USE_DECODE_POLLING
	#include "decode_polling.h"
#endif

extern JPEG_HandleTypeDef hjpeg;

namespace leka {
void JPEGMspInit();
}	// namespace leka

// void OnError_Handler(const char *file, int line);
void OnError_Handler();

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
