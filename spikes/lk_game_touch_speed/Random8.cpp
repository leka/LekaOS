#pragma once

#include <cstdint>
#include <cstdlib>

auto random8(uint8_t min, uint8_t max) -> uint8_t
{
	return min + rand() % (max - min + 1);
}
