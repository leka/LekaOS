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

	static const MagicCard color_purple;
	static const MagicCard color_indigo;
	static const MagicCard color_blue;
	static const MagicCard color_green;
	static const MagicCard color_yellow;
	static const MagicCard color_orange;
	static const MagicCard color_red;

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

	static const MagicCard shape_square;
	static const MagicCard shape_circle;
	static const MagicCard shape_triangle;
	static const MagicCard shape_star;

	static const MagicCard activity_music_quest;
	static const MagicCard activity_super_simon;
	static const MagicCard activity_colored_quest;
	static const MagicCard activity_music_colored_board;
	static const MagicCard activity_hide_and_seek;
	static const MagicCard activity_colors_and_sounds;
	static const MagicCard activity_magic_objects;
	static const MagicCard activity_dance_freeze;

	static const MagicCard remote_standard;
	static const MagicCard remote_colored_arrows;

	static const MagicCard reinforcer_1_blink_green;
	static const MagicCard reinforcer_2_spin_blink;
	static const MagicCard reinforcer_3_fire;
	static const MagicCard reinforcer_4_sprinkles;
	static const MagicCard reinforcer_5_rainbow;

	static const MagicCard emotion_fear_child;
	static const MagicCard emotion_disgust_child;
	static const MagicCard emotion_anger_child;
	static const MagicCard emotion_joy_child;
	static const MagicCard emotion_sadness_child;
	static const MagicCard emotion_fear_leka;
	static const MagicCard emotion_disgust_leka;
	static const MagicCard emotion_anger_leka;
	static const MagicCard emotion_joy_leka;
	static const MagicCard emotion_sadness_leka;

	static const MagicCard vegetable_carrot_orange;
	static const MagicCard vegetable_potato_yellow;
	static const MagicCard vegetable_salad_green;
	static const MagicCard vegetable_mushroom_grey;
	static const MagicCard fruit_strawberry_red;
	static const MagicCard fruit_cherry_pink;
	static const MagicCard fruit_apple_green;
	static const MagicCard fruit_banana_yellow;
	static const MagicCard fruit_grapes_black;

	static const MagicCard math_arithmetic_substraction_sign_minus;
	static const MagicCard math_arithmetic_addition_sign_plus;

  private:
	static constexpr auto id_high_byte_index = 4;
	static constexpr auto id_low_byte_index	 = 5;

	rfid::Tag _tag {};
};

constexpr MagicCard MagicCard::none			  = MagicCard {0x00'00};
constexpr MagicCard MagicCard::emergency_stop = MagicCard {0x00'01};
constexpr MagicCard MagicCard::dice_roll	  = MagicCard {0x00'02};

constexpr MagicCard MagicCard::color_purple = MagicCard {0x00'03};
constexpr MagicCard MagicCard::color_indigo = MagicCard {0x00'04};
constexpr MagicCard MagicCard::color_blue	= MagicCard {0x00'05};
constexpr MagicCard MagicCard::color_green	= MagicCard {0x00'06};
constexpr MagicCard MagicCard::color_yellow = MagicCard {0x00'07};
constexpr MagicCard MagicCard::color_orange = MagicCard {0x00'08};
constexpr MagicCard MagicCard::color_red	= MagicCard {0x00'09};

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

constexpr MagicCard MagicCard::shape_square	  = MagicCard {0x00'15};
constexpr MagicCard MagicCard::shape_circle	  = MagicCard {0x00'16};
constexpr MagicCard MagicCard::shape_triangle = MagicCard {0x00'17};
constexpr MagicCard MagicCard::shape_star	  = MagicCard {0x00'18};

constexpr MagicCard MagicCard::activity_music_quest			= MagicCard {0x00'19};
constexpr MagicCard MagicCard::activity_super_simon			= MagicCard {0x00'1A};
constexpr MagicCard MagicCard::activity_colored_quest		= MagicCard {0x00'1B};
constexpr MagicCard MagicCard::activity_music_colored_board = MagicCard {0x00'1C};
constexpr MagicCard MagicCard::activity_hide_and_seek		= MagicCard {0x00'1D};
constexpr MagicCard MagicCard::activity_colors_and_sounds	= MagicCard {0x00'1E};
constexpr MagicCard MagicCard::activity_magic_objects		= MagicCard {0x00'1F};
constexpr MagicCard MagicCard::activity_dance_freeze		= MagicCard {0x00'20};

constexpr MagicCard MagicCard::remote_standard		 = MagicCard {0x00'21};
constexpr MagicCard MagicCard::remote_colored_arrows = MagicCard {0x00'22};

constexpr MagicCard MagicCard::reinforcer_1_blink_green = MagicCard {0x00'23};
constexpr MagicCard MagicCard::reinforcer_2_spin_blink	= MagicCard {0x00'24};
constexpr MagicCard MagicCard::reinforcer_3_fire		= MagicCard {0x00'25};
constexpr MagicCard MagicCard::reinforcer_4_sprinkles	= MagicCard {0x00'26};
constexpr MagicCard MagicCard::reinforcer_5_rainbow		= MagicCard {0x00'27};

constexpr MagicCard MagicCard::emotion_fear_child	 = MagicCard {0x00'28};
constexpr MagicCard MagicCard::emotion_disgust_child = MagicCard {0x00'29};
constexpr MagicCard MagicCard::emotion_anger_child	 = MagicCard {0x00'2A};
constexpr MagicCard MagicCard::emotion_joy_child	 = MagicCard {0x00'2B};
constexpr MagicCard MagicCard::emotion_sadness_child = MagicCard {0x00'2C};
constexpr MagicCard MagicCard::emotion_fear_leka	 = MagicCard {0x00'2D};
constexpr MagicCard MagicCard::emotion_disgust_leka	 = MagicCard {0x00'2E};
constexpr MagicCard MagicCard::emotion_anger_leka	 = MagicCard {0x00'2F};
constexpr MagicCard MagicCard::emotion_joy_leka		 = MagicCard {0x00'30};
constexpr MagicCard MagicCard::emotion_sadness_leka	 = MagicCard {0x00'31};

constexpr MagicCard MagicCard::vegetable_carrot_orange = MagicCard {0x00'32};
constexpr MagicCard MagicCard::vegetable_potato_yellow = MagicCard {0x00'33};
constexpr MagicCard MagicCard::vegetable_salad_green   = MagicCard {0x00'34};
constexpr MagicCard MagicCard::vegetable_mushroom_grey = MagicCard {0x00'35};
constexpr MagicCard MagicCard::fruit_strawberry_red	   = MagicCard {0x00'36};
constexpr MagicCard MagicCard::fruit_cherry_pink	   = MagicCard {0x00'37};
constexpr MagicCard MagicCard::fruit_apple_green	   = MagicCard {0x00'38};
constexpr MagicCard MagicCard::fruit_banana_yellow	   = MagicCard {0x00'39};
constexpr MagicCard MagicCard::fruit_grapes_black	   = MagicCard {0x00'3A};

constexpr MagicCard MagicCard::math_arithmetic_substraction_sign_minus = MagicCard {0x00'3B};
constexpr MagicCard MagicCard::math_arithmetic_addition_sign_plus	   = MagicCard {0x00'3C};

}	// namespace leka

// ? Make MagicCard std::hash compatible for use in associative containers (i.e. std::unordered_map)
namespace std {
template <>
struct hash<leka::MagicCard> {
	auto operator()(const leka::MagicCard &card) const -> size_t { return hash<int>()(card.getId()); }
};
}	// namespace std
