// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FONT_H_
#define _LEKA_OS_LIB_FONT_H_

#include "CGColor.h"
#include "LKCoreFontBase.h"

namespace leka {

class LKCoreFont : public LKCoreFontBase
{
  public:
	LKCoreFont(LKCoreGraphicsBase &graphics);

	void drawChar(Character character, Color foreground = CGColor::black, Color background = CGColor::white) final;
	void display(char *text, uint16_t text_length, uint8_t starting_line, Color foreground = CGColor::black,
				 Color background = CGColor::white) final;

	const uint8_t *fontGetFirstPixelAddress(char character) final;
	uint32_t fontGetPixelBytes(uint8_t *line_address) final;
	bool fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) final;

  private:
	LKCoreGraphicsBase &_graphics;
};

}	// namespace leka

#endif
