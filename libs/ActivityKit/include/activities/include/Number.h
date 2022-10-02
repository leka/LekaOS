// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "MagicCard.h"

namespace leka::activity {

struct Number {
	MagicCard card;
	const char *id;

	static const Number number_0;
	static const Number number_1;
	static const Number number_2;
	static const Number number_3;
	static const Number number_4;
	static const Number number_5;
	static const Number number_6;
	static const Number number_7;
	static const Number number_8;
	static const Number number_9;
	static const Number number_10;
};

inline constexpr Number Number::number_0  = Number {MagicCard::number_0, "000A"};
inline constexpr Number Number::number_1  = Number {MagicCard::number_1, "000B"};
inline constexpr Number Number::number_2  = Number {MagicCard::number_2, "000C"};
inline constexpr Number Number::number_3  = Number {MagicCard::number_3, "000D"};
inline constexpr Number Number::number_4  = Number {MagicCard::number_4, "000E"};
inline constexpr Number Number::number_5  = Number {MagicCard::number_5, "000F"};
inline constexpr Number Number::number_6  = Number {MagicCard::number_6, "0010"};
inline constexpr Number Number::number_7  = Number {MagicCard::number_7, "0011"};
inline constexpr Number Number::number_8  = Number {MagicCard::number_8, "0012"};
inline constexpr Number Number::number_9  = Number {MagicCard::number_9, "0013"};
inline constexpr Number Number::number_10 = Number {MagicCard::number_10, "0014"};

}	// namespace leka::activity

// LCOV_EXCL_STOP
