// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>
#include <span>

#include "interface/drivers/RFIDReader.h"

namespace leka {

enum class state : uint8_t
{

	SENSOR_SLEEP				   = 0x00,
	TAG_COMMUNICATION_PROTOCOL_SET = 0x01,
	WAIT_FOR_ATQA_RESPONSE		   = 0x02,
	TAG_IDENTIFIED				   = 0x03,
	TAG_ACTIVATED				   = 0x04,

};

enum class Tag : uint8_t
{
	emergency_stop							= 0x01,
	dice_roll								= 0x02,
	color_violet_paint						= 0x03,
	color_indigo_paint						= 0x04,
	color_blue_paint						= 0x05,
	color_green_paint						= 0x06,
	color_yellow_paint						= 0x07,
	color_orange_paint						= 0x08,
	color_red_paint							= 0x09,
	number_0_zero							= 0x0A,
	number_1_one							= 0x0B,
	number_2_two							= 0x0C,
	number_3_three							= 0x0D,
	number_4_four							= 0x0E,
	number_5_five							= 0x0F,
	number_6_six							= 0x10,
	number_7_seven							= 0x11,
	number_8_eight							= 0x12,
	number_9_nine							= 0x13,
	number_10_ten							= 0x14,
	shape_square_plain_line					= 0x15,
	shape_circle_plain_line					= 0x16,
	shape_triangle_plain_line				= 0x17,
	shape_star_plain_line					= 0x18,
	activity_music_quest					= 0x19,
	activity_super_simon					= 0x1A,
	activity_color_quest					= 0x1B,
	activity_music_colored_board			= 0x1C,
	activity_hide_and_seek					= 0x1D,
	activity_colors_and_sounds				= 0x1E,
	activity_magic_objects					= 0x1F,
	activity_dance_freeze					= 0x20,
	remote_standard							= 0x21,
	remote_colored_arrows					= 0x22,
	reinforcer_1_green_spin					= 0x23,
	reinforcer_2_violet_green_blink_spin	= 0x24,
	reinforcer_3_fire_static				= 0x25,
	reinforcer_4_glitters_static			= 0x26,
	reinforcer_5_rainbow_static				= 0x27,
	emotion_fear_child						= 0x28,
	emotion_disgust_child					= 0x29,
	emotion_anger_child						= 0x2A,
	emotion_joy_child						= 0x2B,
	emotion_sadness_child					= 0x2C,
	emotion_fear_leka						= 0x2D,
	emotion_disgust_leka					= 0x2E,
	emotion_anger_leka						= 0x2F,
	emotion_joy_leka						= 0x30,
	emotion_sadness_leka					= 0x31,
	vegetable_carrot_orange					= 0x32,
	vegetable_potato_yellow					= 0x33,
	vegetable_salad_green					= 0x34,
	vegetable_mushroom_grey					= 0x35,
	fruit_strawberry_red					= 0x36,
	fruit_cherry_pink						= 0x37,
	fruit_apple_green						= 0x38,
	fruit_banana_yellow						= 0x39,
	fruit_grapes_black						= 0x3A,
	math_arithmetic_substraction_sign_minus = 0x3B,
	math_arithmetic_addition_sign_plus		= 0x3C,
	None
};

class RFIDKit : public interface::RFIDReader::ISO14443
{
  public:
	explicit RFIDKit(interface::RFIDReader &rfid_reader) : _rfid_reader(rfid_reader) {};

	void init() final;

	void getTagData() final;

	auto getTag() -> rfid::Tag final { return _tag; };

	void resetTag() { _tag.data = {}; };

	void onTagActivated(std::function<void(rfid::Tag &)> const &callback);

  private:
	template <size_t SIZE>
	void commandToArray(Command<SIZE> command, std::span<uint8_t> span) const
	{
		for (size_t i = 0; i < SIZE; ++i) {
			span.data()[i] = command.data.data()[i];
		}

		span.data()[SIZE] = static_cast<uint8_t>(command.flags);
	}

	void sendREQA();
	void sendReadRegister0();
	void sendReadRegister4();

	auto receiveATQA() -> bool;
	auto receiveReadTagData() -> bool;

	auto isTagSignatureValid() -> bool;

	auto computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>;

	void getData(std::array<uint8_t, 16> &tag_data);

	interface::RFIDReader &_rfid_reader;
	rfid::Tag _tag {};
	std::function<void(rfid::Tag &)> _on_tag_activated_callback;
	state _state = state::SENSOR_SLEEP;
};

}	// namespace leka
