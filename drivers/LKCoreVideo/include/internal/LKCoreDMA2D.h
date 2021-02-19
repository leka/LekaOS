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

	void transferImage(uint16_t width, uint16_t height, uint16_t width_offset);

  private:
	DMA2D_HandleTypeDef _hdma2d;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
