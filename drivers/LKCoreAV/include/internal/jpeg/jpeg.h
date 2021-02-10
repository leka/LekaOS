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
#include "dma2d.h"
#include "lcd_properties.h"
#include "st_jpeg_utils.h"

#if USE_DECODE_DMA
	#include "st_decode_dma.h"
#elif USE_DECODE_POLLING
	#include "st_decode_polling.h"
#elif USE_DECODE_INTERRUPT
	#include "st_decode_interrupt.h"
#endif

namespace leka {

class LKCoreJPEG
{
  public:
	LKCoreJPEG();

	void initialize();
	void disposeHAL();

	void display(FIL *JPEG_File);

  private:
	void decode(JPEG_HandleTypeDef *hjpeg, FIL *file, uint32_t DestAddress);

	JPEG_HandleTypeDef hjpeg;
	JPEG_ConfTypeDef hjpeginfo;

#if USE_DECODE_INTERRUPT or USE_DECODE_DMA
	uint32_t JpegProcessing_End = 0;
#endif
};

}	// namespace leka

#if USE_DECODE_INTERRUPT
void JPEG_IRQHandler(void);
#endif
#if USE_DECODE_DMA
void JPEG_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);
#endif

void OnError_Handler();

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
