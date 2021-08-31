// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FONT_BASE_H_
#define _LEKA_OS_LIB_FONT_BASE_H_

// #include <cstdint>

#include "CGColor.h"
#include "CGPixel.h"

namespace leka {

class LKCoreFontBase
{
  public:
	struct Character {
		Point origin;	 // Top left corner by convention
		uint8_t ascii;	 // From 0x20 to 0x7F
	};

	virtual ~LKCoreFontBase() = default;

	virtual void drawChar(Character character, CGColor foreground = CGColor::black,
						  CGColor background = CGColor::white) = 0;
	virtual void display(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
						 CGColor background = CGColor::white)  = 0;

	virtual const uint8_t *fontGetFirstPixelAddress(char character)		= 0;
	virtual uint32_t fontGetPixelBytes(const uint8_t *line_address)		= 0;
	virtual bool fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) = 0;
};

}	// namespace leka

#endif
