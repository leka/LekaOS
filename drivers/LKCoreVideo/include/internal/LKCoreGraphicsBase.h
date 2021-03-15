// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_BASE_H_
#define _LEKA_OS_LIB_GRAPHICS_BASE_H_

#include "CGColor.h"

namespace leka {

class LKCoreGraphicsBase
{
  public:
	struct Pixel {
		uint16_t x = 0;
		uint16_t y = 0;
	};

	struct FilledRectangle {
		Pixel origin;	// Top left corner by convention
		uint16_t width;
		uint16_t height;
	};

	virtual ~LKCoreGraphicsBase() = default;

	virtual void clearScreen(Color color = CGColor::white) = 0;

	virtual void drawPixel(Pixel pixel, Color color)				   = 0;
	virtual void drawRectangle(FilledRectangle rectangle, Color color) = 0;

	virtual uintptr_t getDestinationAddress() = 0;
	virtual uint32_t getDestinationColor()	  = 0;
};

}	// namespace leka

#endif
