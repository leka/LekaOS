// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

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
		red	  = static_cast<uint8_t>(std::min(red + rhs.red, 255));
		green = static_cast<uint8_t>(std::min(green + rhs.green, 255));
		blue  = static_cast<uint8_t>(std::min(blue + rhs.blue, 255));

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
	static const RGB purple;
	static const RGB orange;
};

constexpr RGB RGB::white {0xFF, 0xFF, 0xFF};
constexpr RGB RGB::black {0x00, 0x00, 0x00};

constexpr RGB RGB::pure_red {0xFF, 0x00, 0x00};
constexpr RGB RGB::pure_green {0x00, 0xFF, 0x00};
constexpr RGB RGB::pure_blue {0x00, 0x00, 0xFF};

constexpr RGB RGB::yellow {0xFF, 0xFF, 0x00};
constexpr RGB RGB::cyan {0x00, 0xFF, 0xFF};
constexpr RGB RGB::magenta {0xFF, 0x00, 0xFF};

constexpr RGB RGB::purple {0x14, 0x00, 0x50};
constexpr RGB RGB::orange {0xFF, 0x80, 0x00};

}	// namespace leka
