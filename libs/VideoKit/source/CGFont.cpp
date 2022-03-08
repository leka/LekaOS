#include "CGFont.hpp"

using namespace leka;

auto CGFont::getCharAddress(char character) -> const uint8_t *
{
	if (character < CGFont::first_char || character > CGFont::last_char) {
		return CGFont::table;
	}

	auto char_index = character - CGFont::first_char;
	return CGFont::table + CGFont::bytes_per_char * char_index;
}

auto CGFont::isPixelOn(const uint8_t *char_address, uint32_t x, uint32_t y) -> bool
{
	char_address += CGFont::bytes_per_line * y;
	uint32_t char_line = (((*char_address) << 16 | (*(char_address + 1)) << 8 | (*(char_address + 2)) << 0) >> 7);
	return (char_line >> (CGFont::character_width - x) & 1) > 0;
}
