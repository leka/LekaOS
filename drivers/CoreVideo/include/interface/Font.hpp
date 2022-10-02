// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// #include <cstdint>

#include "CGColor.hpp"
#include "CGPixel.hpp"

namespace leka::interface {

class Font
{
  public:
	struct Character {
		CGPoint origin {};	 // Top left corner by convention
		uint8_t ascii {};	 // From 0x20 to 0x7F
	};

	virtual ~Font() = default;

	virtual void drawChar(Character character, CGColor foreground = CGColor::black,
						  CGColor background = CGColor::white) = 0;
	virtual void display(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
						 CGColor background = CGColor::white)  = 0;

	virtual auto fontGetFirstPixelAddress(char character) -> const uint8_t * = 0;
	virtual auto fontGetPixelBytes(const uint8_t *line_address) -> uint32_t	 = 0;

	virtual auto fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) -> bool = 0;
};

}	// namespace leka::interface
