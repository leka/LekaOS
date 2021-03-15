// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreGraphics.h"

#include "LKFont.h"

namespace leka {

LKCoreGraphics::LKCoreGraphics(LKCoreDMA2DBase &dma2d) : _dma2d(dma2d) {}

void LKCoreGraphics::clearScreen(Color color)
{
	FilledRectangle rect;
	rect.width	= lcd::dimension.width;
	rect.height = lcd::dimension.height;

	draw(rect, color);
}

void LKCoreGraphics::draw(Pixel pixel, Color color)
{
	// TODO: Use a buffer and draw all in the same time. Need to determine buffer address
	rawMemoryWrite(pixel, color);
}

const uint8_t *LKCoreGraphics::fontGetFirstPixelAddress(char character)
{
	uint8_t space_ascii_value	  = 0x20;
	uint8_t character_ascii_value = character;
	auto index = (character_ascii_value - space_ascii_value) * graphics::lines_per_char * graphics::bytes_per_line;
	return &LKFontTable[index];
}

uint32_t LKCoreGraphics::fontGetPixelBytes(uint8_t *line_address)
{
	return (line_address[0] << 16) | (line_address[1] << 8) | line_address[2];
}

bool LKCoreGraphics::fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id)
{
	uint32_t pixel_id_mask = (0x01 << ((graphics::pixels_per_line - 1) - pixel_id + graphics::unused_bits));
	return byte_of_line & pixel_id_mask;   // Return true if different of 0x00
}

void LKCoreGraphics::draw(Character character, Color foreground, Color background)
{
	Pixel pixel_to_draw;
	pixel_to_draw.x = character.origin.x;
	pixel_to_draw.y = character.origin.y;

	const uint8_t *first_byte_address = fontGetFirstPixelAddress(character.ascii);

	for (uint32_t next_byte = 0; next_byte < graphics::bytes_per_char; next_byte += graphics::bytes_per_line) {
		uint32_t byte_of_line = fontGetPixelBytes((uint8_t *)first_byte_address + next_byte);

		for (uint8_t pixel_id = 0; pixel_id < graphics::pixels_per_line; pixel_id++) {
			pixel_to_draw.x = character.origin.x + pixel_id;

			if (fontPixelIsOn(byte_of_line, pixel_id)) {
				draw(pixel_to_draw, foreground);
			} else {
				draw(pixel_to_draw, background);
			}
		}
		pixel_to_draw.y++;
	}
}

void LKCoreGraphics::display(char *text, uint16_t text_length, uint8_t starting_line, Color foreground,
							 Color background)
{
	if (starting_line > 19) {
		return;
	}

	Character character;
	character.origin.x = 0;
	character.origin.y = starting_line * graphics::font_pixel_height;

	for (uint16_t char_index = 0; char_index < text_length; char_index++) {
		if (text[char_index] == '\n') {
			character.origin.y += graphics::font_pixel_height;
			character.origin.x = 0;
		} else if (text[char_index] < ' ') {
			// Character non supported before first writtable character
			continue;
		} else if (text[char_index] > '~') {
			// Character non supported beyond last writtable character
			continue;
		} else {
			character.ascii = text[char_index];
			draw(character, foreground, background);

			character.origin.x += graphics::font_pixel_width;
		}

		if ((character.origin.x + graphics::font_pixel_width) > lcd::dimension.width) {
			character.origin.y += graphics::font_pixel_height;
			character.origin.x = 0;
		}
		if ((character.origin.y + graphics::font_pixel_height) > lcd::dimension.height) {
			return;
		}
	}
}

void LKCoreGraphics::draw(FilledRectangle rectangle, Color color)
{
	uintptr_t first_pixel_address =
		lcd::frame_buffer_address + 4 * (lcd::dimension.width * rectangle.origin.y + rectangle.origin.x);

	_dma2d.transferDrawing(first_pixel_address, rectangle.width, rectangle.height, color.getARGB());
}

}	// namespace leka
