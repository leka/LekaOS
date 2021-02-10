// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DMA2D_H_
#define _LEKA_OS_LIB_DMA2D_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreDMA2D
{
  public:
	LKCoreDMA2D();

	void initialize();

	static void loadImage(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,
						  uint32_t width_offset, uint16_t screen_width = 800);
	static void loadDrawing(void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);

  private:
	static DMA2D_HandleTypeDef hdma2d;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
