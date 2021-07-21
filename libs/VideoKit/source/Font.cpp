#include "Font.h"

using namespace leka;

auto Font::getCharAddress(char character) -> const uint8_t *
{
	if (character < Font::first_char || character > Font::last_char) {
		return Font::table;
	}

	auto char_index = character - Font::first_char;
	return Font::table + Font::bytes_per_char * char_index;
}

auto Font::isPixelOn(const uint8_t *char_address, uint32_t x, uint32_t y) -> bool
{
	char_address += Font::bytes_per_line * y;
	uint32_t char_line = (((*char_address) << 16 | (*(char_address + 1)) << 8 | (*(char_address + 2)) << 0) >> 7);
	return (char_line >> (Font::character_width - x) & 1) > 0;
}
