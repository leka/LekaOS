// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "MemoryUtils.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

struct MagicCard {
	explicit constexpr MagicCard(uint16_t id)
	{
		_tag.data.at(id_high_byte_index) = utils::memory::getHighByte(id);
		_tag.data.at(id_low_byte_index)	 = utils::memory::getLowByte(id);
	}

	explicit constexpr MagicCard(const rfid::Tag &tag) : _tag(tag) {}

	[[nodiscard]] auto getId() const -> uint16_t
	{
		auto high = _tag.data.at(id_high_byte_index);
		auto low  = _tag.data.at(id_low_byte_index);

		auto both = utils::memory::combineBytes({.high = high, .low = low});

		return both;
	}

	constexpr auto operator==(MagicCard const &rhs) const -> bool
	{
		auto same_high = _tag.data.at(id_high_byte_index) == rhs._tag.data.at(id_high_byte_index);
		auto same_low  = _tag.data.at(id_low_byte_index) == rhs._tag.data.at(id_low_byte_index);

		auto same = same_high && same_low;

		return same;
	}

	static const MagicCard none;
	static const MagicCard emergency_stop;
	static const MagicCard dice_roll;

	static const MagicCard number_0;
	static const MagicCard number_1;
	static const MagicCard number_2;
	static const MagicCard number_3;
	static const MagicCard number_4;
	static const MagicCard number_5;
	static const MagicCard number_6;
	static const MagicCard number_7;
	static const MagicCard number_8;
	static const MagicCard number_9;
	static const MagicCard number_10;

  private:
	static constexpr auto id_high_byte_index = 4;
	static constexpr auto id_low_byte_index	 = 5;

	rfid::Tag _tag {};
};

constexpr MagicCard MagicCard::none			  = MagicCard {0x00'00};
constexpr MagicCard MagicCard::emergency_stop = MagicCard {0x00'01};
constexpr MagicCard MagicCard::dice_roll	  = MagicCard {0x00'02};

constexpr MagicCard MagicCard::number_0	 = MagicCard {0x00'0A};
constexpr MagicCard MagicCard::number_1	 = MagicCard {0x00'0B};
constexpr MagicCard MagicCard::number_2	 = MagicCard {0x00'0C};
constexpr MagicCard MagicCard::number_3	 = MagicCard {0x00'0D};
constexpr MagicCard MagicCard::number_4	 = MagicCard {0x00'0E};
constexpr MagicCard MagicCard::number_5	 = MagicCard {0x00'0F};
constexpr MagicCard MagicCard::number_6	 = MagicCard {0x00'10};
constexpr MagicCard MagicCard::number_7	 = MagicCard {0x00'11};
constexpr MagicCard MagicCard::number_8	 = MagicCard {0x00'12};
constexpr MagicCard MagicCard::number_9	 = MagicCard {0x00'13};
constexpr MagicCard MagicCard::number_10 = MagicCard {0x00'14};

}	// namespace leka
