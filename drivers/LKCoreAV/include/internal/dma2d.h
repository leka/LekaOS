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

	static void load(bool is_image, uint32_t source_address_color, uint32_t destination_address, uint16_t xsize,
					 uint16_t ysize, uint16_t output_offset, uint32_t width_offset = 0);

  private:
	static DMA2D_HandleTypeDef hdma2d;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
