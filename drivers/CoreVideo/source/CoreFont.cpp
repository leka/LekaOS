// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFont.hpp"

#include "CGFont.hpp"
#include "internal/corevideo_config.h"

using namespace leka;

CoreFont::CoreFont(CGPixel &pixel_to_draw) : _pixel_to_draw(pixel_to_draw) {}

auto CoreFont::fontGetFirstPixelAddress(char character) -> const uint8_t *
{
	uint8_t space_ascii_value	  = 0x20;
	uint8_t character_ascii_value = character;
	auto index = (character_ascii_value - space_ascii_value) * graphics::lines_per_char * graphics::bytes_per_line;
	return &CGFontTable.at(index);
}

auto CoreFont::fontGetPixelBytes(const uint8_t *line_address) -> uint32_t
{
	return (line_address[0] << 16) | (line_address[1] << 8) | line_address[2];
}

auto CoreFont::fontPixelIsOn(uint32_t byte_of_line, uint8_t pixel_id) -> bool
{
	uint32_t pixel_id_mask = (0x01 << ((graphics::pixels_per_line - 1) - pixel_id + graphics::unused_bits));

	auto is_on = (byte_of_line & pixel_id_mask) != 0U;

	return is_on;
}

void CoreFont::drawChar(CGCharacter character, CGColor foreground, CGColor background)
{
	_pixel_to_draw.coordinates.x = character.origin.x;
	_pixel_to_draw.coordinates.y = character.origin.y;

	const uint8_t *first_byte_address = fontGetFirstPixelAddress(character.ascii);

	for (uint32_t next_byte = 0; next_byte < graphics::bytes_per_char; next_byte += graphics::bytes_per_line) {
		uint32_t byte_of_line = fontGetPixelBytes(first_byte_address + next_byte);

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

void CoreFont::display(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground, CGColor background)
{
	if (starting_line < 1 || starting_line > 20) {
		return;
	}

	auto character	   = CGCharacter {};
	character.origin.x = 0;
	character.origin.y = (starting_line - 1) * graphics::font_pixel_height;

	for (uint32_t char_index = 0; char_index < size; char_index++) {
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

		if ((character.origin.x + graphics::font_pixel_width) > lcd::dimension::width) {
			character.origin.y += graphics::font_pixel_height;
			character.origin.x = 0;
		}
		if ((character.origin.y + graphics::font_pixel_height) > lcd::dimension::height) {
			return;
		}
	}
}

auto CoreFont::getLastDrawnPixel() const -> CGPixel
{
	return _pixel_to_draw;
}
