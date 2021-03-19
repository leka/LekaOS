// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_BASE_H_
#define _LEKA_OS_LIB_GRAPHICS_BASE_H_

#include "CGColor.h"
#include "CGPixel.h"

namespace leka {

class LKCoreGraphicsBase
{
  public:
	struct FilledRectangle {
		Point origin;	// Top left corner by convention
		uint16_t width;
		uint16_t height;
	};

	virtual ~LKCoreGraphicsBase() = default;

	virtual void clearScreen(Color color = CGColor::white) = 0;

	virtual void drawRectangle(FilledRectangle rectangle, Color color) = 0;
};

}	// namespace leka

#endif
