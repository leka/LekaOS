// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CGColor.hpp"
#include "CGPixel.hpp"

namespace leka::interface {

class Graphics
{
  public:
	struct FilledRectangle {
		CGPoint origin {0, 0};	 // * Top left corner by convention
		uint16_t width {};
		uint16_t height {};
	};

	virtual ~Graphics() = default;

	virtual void clearScreen(CGColor color = CGColor::white) = 0;

	virtual void drawRectangle(FilledRectangle rectangle, CGColor color) = 0;
};

}	// namespace leka::interface
