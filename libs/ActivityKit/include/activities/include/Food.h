// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "MagicCard.h"

namespace leka::activity {

struct Food {
	MagicCard card;
	const char *id;

	static const Food carrot;
	static const Food potato;
	static const Food salad;
	static const Food mushroom;
	static const Food strawberry;
	static const Food cherry;
	static const Food apple;
	static const Food banana;
	static const Food grapes;
};

inline constexpr Food Food::carrot	   = Food {MagicCard::vegetable_carrot_orange, "0032"};
inline constexpr Food Food::potato	   = Food {MagicCard::vegetable_potato_yellow, "0033"};
inline constexpr Food Food::salad	   = Food {MagicCard::vegetable_salad_green, "0034"};
inline constexpr Food Food::mushroom   = Food {MagicCard::vegetable_mushroom_grey, "0035"};
inline constexpr Food Food::strawberry = Food {MagicCard::fruit_strawberry_red, "0036"};
inline constexpr Food Food::cherry	   = Food {MagicCard::fruit_cherry_pink, "0037"};
inline constexpr Food Food::apple	   = Food {MagicCard::fruit_apple_green, "0038"};
inline constexpr Food Food::banana	   = Food {MagicCard::fruit_banana_yellow, "0039"};
inline constexpr Food Food::grapes	   = Food {MagicCard::fruit_grapes_black, "003A"};

}	// namespace leka::activity

// LCOV_EXCL_STOP
