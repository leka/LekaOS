// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FONT_H_
#define _LEKA_OS_LIB_FONT_H_

#include "CGColor.h"
#include "CGPixel.h"
#include "LKCoreFontBase.h"

namespace leka {

class LKCoreFont : public LKCoreFontBase
{
  public:
	LKCoreFont(CGPixel &pixel_to_draw);

	void drawChar(Character character, Color foreground = CGColor::black, Color background = CGColor::white) final;
	void display(char *text, uint16_t text_length, uint8_t starting_line, Color foreground = CGColor::black,
				 Color background = CGColor::white) final;

	const uint8_t *fontGetFirstPixelAddress(char character) final;
	uint32_t fontGetPixelBytes(uint8_t *line_address) final;
	bool fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) final;

  private:
	CGPixel &_pixel_to_draw;
};

}	// namespace leka

#endif
