// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_BASE_H_
#define _LEKA_OS_LIB_GRAPHICS_BASE_H_

#include "CGColor.h"
#include "CGPixel.h"

namespace leka::interface {

class LKCoreGraphics
{
  public:
	struct FilledRectangle {
		Point origin;	// Top left corner by convention
		uint16_t width;
		uint16_t height;
	};

	virtual ~LKCoreGraphics() = default;

	virtual void clearScreen(CGColor color = CGColor::white) = 0;

	virtual void drawRectangle(FilledRectangle rectangle, CGColor color) = 0;
};

}	// namespace leka::interface

#endif
