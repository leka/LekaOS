// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_BASE_H_
#define _LEKA_OS_LIB_GRAPHICS_BASE_H_

#include "CGColor.hpp"
#include "CGPixel.hpp"

namespace leka::interface {

class CoreGraphics
{
  public:
	struct FilledRectangle {
		CGPoint origin;	  // Top left corner by convention
		uint16_t width;
		uint16_t height;
	};

	virtual ~CoreGraphics() = default;

	virtual void clearScreen(CGColor color = CGColor::white) = 0;

	virtual void drawRectangle(FilledRectangle rectangle, CGColor color) = 0;
};

}	// namespace leka::interface

#endif
