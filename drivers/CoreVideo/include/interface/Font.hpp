// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

struct CGColor;
struct CGCharacter;

}	// namespace leka

namespace leka::interface {

class Font
{
  public:
	virtual ~Font() = default;

	virtual void drawChar(CGCharacter character, CGColor foreground, CGColor background) = 0;
	virtual void display(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground,
						 CGColor background)											 = 0;

	virtual auto fontGetFirstPixelAddress(char character) -> const uint8_t * = 0;
	virtual auto fontGetPixelBytes(const uint8_t *line_address) -> uint32_t	 = 0;

	virtual auto fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) -> bool = 0;
};

}	// namespace leka::interface
