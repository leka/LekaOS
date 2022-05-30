// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "interface/drivers/RFIDReader.h"

namespace leka {

struct MagicCard {
	explicit constexpr MagicCard(uint8_t id) { _tag.data.at(id_index) = id; }
	explicit constexpr MagicCard(const rfid::Tag &tag) : _tag(tag) {}

	[[nodiscard]] auto getId() const -> uint8_t { return _tag.data.at(id_index); }

	auto operator==(MagicCard const &rhs) const -> bool { return _tag.data.at(id_index) == rhs._tag.data.at(id_index); }

	static const MagicCard none;
	static const MagicCard emergency_stop;
	static const MagicCard dice_roll;

  private:
	static constexpr auto id_index = 5;
	rfid::Tag _tag {};
};

constexpr MagicCard MagicCard::none {0x00};
constexpr MagicCard MagicCard::emergency_stop {0x01};
constexpr MagicCard MagicCard::dice_roll {0x02};

}	// namespace leka
