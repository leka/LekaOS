#ifndef _LEKA_OS_LIB_GRAPHICS_H
#define _LEKA_OS_LIB_GRAPHICS_H

#include <cstdint>

namespace leka::gfx {

struct Color {
	uint8_t r = 0x00;
	uint8_t g = 0x00;
	uint8_t b = 0x00;
	uint8_t a = 0xff;

	auto toARGB8888() -> uint32_t;

	static Color White;
	static Color Black;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Yellow;
	static Color Cyan;
	static Color Magenta;
};

struct Rectangle {
	uint32_t width;
	uint32_t height;
	Color color;
};

}	// namespace leka::gfx

#endif	 // _LEKA_OS_LIB_GRAPHICS_H
