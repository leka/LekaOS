// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_LED_RGB_H_
#define _LEKA_OS_DRIVER_CORE_LED_RGB_H_

#include <algorithm>
#include <cstdint>

namespace leka {

struct RGB {
	uint8_t red {};
	uint8_t green {};
	uint8_t blue {};

	auto operator<=>(RGB const &rhs) const -> bool = default;

	auto operator+=(RGB const &rhs) -> RGB
	{
		red	  = std::min(red + rhs.red, 255);
		green = std::min(green + rhs.green, 255);
		blue  = std::min(blue + rhs.blue, 255);
		return *this;
	}

	auto operator+(const RGB &rhs) const -> RGB
	{
		auto color = *this;
		color += rhs;
		return color;
	}

	static const RGB white;
	static const RGB black;
	static const RGB pure_red;
	static const RGB pure_green;
	static const RGB pure_blue;
	static const RGB yellow;
	static const RGB cyan;
	static const RGB magenta;
};

constexpr RGB RGB::white {0xFF, 0xFF, 0xFF};
constexpr RGB RGB::black {0x00, 0x00, 0x00};

constexpr RGB RGB::pure_red {0xFF, 0x00, 0x00};
constexpr RGB RGB::pure_green {0x00, 0xFF, 0x00};
constexpr RGB RGB::pure_blue {0x00, 0x00, 0xFF};

constexpr RGB RGB::yellow {0xFF, 0xFF, 0x00};
constexpr RGB RGB::cyan {0x00, 0xFF, 0xFF};
constexpr RGB RGB::magenta {0xFF, 0x00, 0xFF};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_LED_RGB_H_
