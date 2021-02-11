// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DRAWING_H_
#define _LEKA_OS_LIB_DRAWING_H_

#include "dma2d.h"
#include "lcd_properties.h"

namespace leka {

class Draw
{
  public:
	Draw();

	static void clear(uint32_t color = 0xFFFFFFFF);
	static void filledRectangle(uint16_t x_0, uint16_t y_0, uint16_t width, uint16_t height, uint32_t color);

  private:
	static uint32_t frame_buffer_start_address;
	static uint32_t data_address;
};

}	// namespace leka

#endif
