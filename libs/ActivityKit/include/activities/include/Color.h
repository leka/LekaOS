// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "MagicCard.h"
#include "RGB.h"
namespace leka::activity {

struct Color {
	MagicCard card;
	RGB color;
	const char *id;

	static const Color purple;
	static const Color indigo;
	static const Color blue;
	static const Color green;
	static const Color yellow;
	static const Color orange;
	static const Color red;
};

inline constexpr Color Color::purple = Color {MagicCard::color_purple, RGB::purple, "0003"};
inline constexpr Color Color::blue	 = Color {MagicCard::color_blue, RGB::pure_blue, "0005"};
inline constexpr Color Color::green	 = Color {MagicCard::color_green, RGB::pure_green, "0006"};
inline constexpr Color Color::yellow = Color {MagicCard::color_yellow, RGB::yellow, "0007"};
inline constexpr Color Color::orange = Color {MagicCard::color_orange, RGB::orange, "0008"};
inline constexpr Color Color::red	 = Color {MagicCard::color_red, RGB::pure_red, "0009"};

}	// namespace leka::activity

// LCOV_EXCL_STOP
