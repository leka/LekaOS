#include "LogKit.h"
#include "Font.h"


using namespace leka;

auto Font::getCharAddress(char character) -> const uint8_t *
{
	if (character < Font::first_char || character > Font::last_char)
		return Font::table;

	auto i = character - Font::first_char;
	return Font::table + Font::bytes_per_char * i;
}

auto Font::isPixelOn(const uint8_t *char_address, uint32_t x, uint32_t y) -> bool
{
	//log_info("%d -> %d", table, char_address);
	char_address += sizeof(decltype(*char_address)) * 3 * y;
	uint32_t char_line = (((*char_address) << 16 | (*(char_address + 1)) << 8 | (*(char_address + 2)) << 0) >> 7);
	return (char_line >>(Font::character_width - x) & 1) >= 1;
}
