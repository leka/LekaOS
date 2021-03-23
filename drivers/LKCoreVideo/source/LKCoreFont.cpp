// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreFont.h"

#include "LKFont.h"
#include "corevideo_config.h"

namespace leka {

LKCoreFont::LKCoreFont(CGPixel &pixel_to_draw) : _pixel_to_draw(pixel_to_draw) {}

const uint8_t *LKCoreFont::fontGetFirstPixelAddress(char character)
{
	uint8_t space_ascii_value	  = 0x20;
	uint8_t character_ascii_value = character;
	auto index = (character_ascii_value - space_ascii_value) * graphics::lines_per_char * graphics::bytes_per_line;
	return &LKFontTable[index];
}

uint32_t LKCoreFont::fontGetPixelBytes(uint8_t *line_address)
{
	return (line_address[0] << 16) | (line_address[1] << 8) | line_address[2];
}

bool LKCoreFont::fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id)
{
	uint32_t pixel_id_mask = (0x01 << ((graphics::pixels_per_line - 1) - pixel_id + graphics::unused_bits));
	return byte_of_line & pixel_id_mask;   // Return true if different of 0x00
}

void LKCoreFont::drawChar(Character character, Color foreground, Color background)
{
	_pixel_to_draw.coordinates.x = character.origin.x;
	_pixel_to_draw.coordinates.y = character.origin.y;

	const uint8_t *first_byte_address = fontGetFirstPixelAddress(character.ascii);

	for (uint32_t next_byte = 0; next_byte < graphics::bytes_per_char; next_byte += graphics::bytes_per_line) {
		uint32_t byte_of_line = fontGetPixelBytes((uint8_t *)first_byte_address + next_byte);

		for (uint8_t pixel_id = 0; pixel_id < graphics::pixels_per_line; pixel_id++) {
			_pixel_to_draw.coordinates.x = character.origin.x + pixel_id;

			if (fontPixelIsOn(byte_of_line, pixel_id)) {
				_pixel_to_draw.draw(foreground);
			} else {
				_pixel_to_draw.draw(background);
			}
		}
		_pixel_to_draw.coordinates.y++;
	}
}

void LKCoreFont::display(char *text, uint16_t text_length, uint8_t starting_line, Color foreground, Color background)
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
		} else {
			character.ascii = text[char_index];
			drawChar(character, foreground, background);

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

}	// namespace leka
