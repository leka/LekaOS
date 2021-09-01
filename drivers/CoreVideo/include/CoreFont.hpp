// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FONT_H_
#define _LEKA_OS_LIB_FONT_H_

#include "CGColor.hpp"
#include "CGPixel.hpp"
#include "interface/CoreFont.hpp"

namespace leka {

class CoreFont : public interface::CoreFont
{
  public:
	explicit CoreFont(CGPixel &pixel_to_draw);

	void drawChar(Character character, CGColor foreground = CGColor::black, CGColor background = CGColor::white) final;
	void display(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
				 CGColor background = CGColor::white) final;

	const uint8_t *fontGetFirstPixelAddress(char character) final;
	uint32_t fontGetPixelBytes(const uint8_t *line_address) final;
	bool fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) final;

	CGPixel getLastDrawnPixel() const;

  private:
	CGPixel &_pixel_to_draw;
};

}	// namespace leka

#endif
