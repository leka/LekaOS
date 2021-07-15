#include "LKGraphics.h"

using namespace leka;
using namespace leka::gfx;

auto Color::toARGB8888() -> uint32_t
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

Color Color::White	 = {0xff, 0xff, 0xff};
Color Color::Black	 = {0x00, 0x00, 0x00};
Color Color::Red	 = {0xff, 0x00, 0x00};
Color Color::Green	 = {0x00, 0xff, 0x00};
Color Color::Blue	 = {0x00, 0x00, 0xff};
Color Color::Yellow	 = {0xff, 0xff, 0x00};
Color Color::Cyan	 = {0x00, 0xff, 0xff};
Color Color::Magenta = {0xff, 0x00, 0xff};
