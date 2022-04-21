// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

struct MagicCard {
	uint8_t data {};

	auto operator<=>(MagicCard const &rhs) const -> bool = default;

	static const MagicCard None;
	static const MagicCard emergency_stop;
	static const MagicCard dice_roll;
};

constexpr MagicCard MagicCard::None {0x00};
constexpr MagicCard MagicCard::emergency_stop {0x01};
constexpr MagicCard MagicCard::dice_roll {0x02};

}	// namespace leka
