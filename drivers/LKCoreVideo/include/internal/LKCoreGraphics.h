// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_H_
#define _LEKA_OS_LIB_GRAPHICS_H_

#include "CGColor.h"
#include "LKCoreDMA2D.h"
#include "corevideo_config.h"

namespace leka {

class LKCoreGraphics
{
  public:
	struct Pixel {
		uint16_t x = 0;
		uint16_t y = 0;
	};

	struct Character {
		Pixel origin;	 // Top left corner by convention
		uint8_t ascii;	 // From 0x20 to 0x7F
	};

	struct FilledRectangle {
		Pixel origin;	// Top left corner by convention
		uint16_t width;
		uint16_t height;
	};

	LKCoreGraphics(LKCoreDMA2D &dma2d);

	void clearScreen(Color color = CGColor::white);

	void draw(Pixel pixel, Color color);
	void draw(Character character, Color foreground = CGColor::black, Color background = CGColor::white);
	void display(char *text, uint16_t text_length, uint8_t starting_line, Color foreground = CGColor::black,
				 Color background = CGColor::white);
	void draw(FilledRectangle rectangle, Color color);

	const uint8_t *fontGetFirstPixelAddress(char character);
	uint32_t fontGetPixelBytes(uint8_t *line_address);
	bool fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id);

  private:
	void rawMemoryWrite(Pixel pixel, Color color)
	{
		*(uint32_t *)(lcd::frame_buffer_address + (4 * (pixel.y * lcd::dimension.width + pixel.x))) = color.getARGB();
	}

  private:
	uintptr_t _data_address;
	LKCoreDMA2D _dma2d;
};

}	// namespace leka

#endif
