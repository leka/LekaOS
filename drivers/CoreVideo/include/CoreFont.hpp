// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CGGraphics.hpp"
#include "interface/Font.hpp"

namespace leka {

class CoreFont : public interface::Font
{
  public:
	explicit CoreFont(CGPixel &pixel_to_draw);

	void drawChar(CGCharacter character, CGColor foreground = CGColor::black,
				  CGColor background = CGColor::white) final;
	void display(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
				 CGColor background = CGColor::white) final;

	auto fontGetFirstPixelAddress(char character) -> const uint8_t * final;
	auto fontGetPixelBytes(const uint8_t *line_address) -> uint32_t final;
	auto fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) -> bool final;

	[[nodiscard]] auto getLastDrawnPixel() const -> CGPixel;

  private:
	CGPixel &_pixel_to_draw;
};

}	// namespace leka
