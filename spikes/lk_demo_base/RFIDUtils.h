// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_RFID_UTILS_H_
#define _LEKA_OS_RFID_UTILS_H_

#include "rtos/EventFlags.h"

#include "CoreBufferedSerial.h"
#include "CoreCR95HF.h"
#include "Flags.h"
#include "RFIDKit.h"

namespace leka {

constexpr std::array<char, 4> rfid_tag_header = {'L', 'E', 'K', 'A'};

enum class Tag : uint8_t
{
	shape_circle_plain_line				 = 0x01,
	shape_square_plain_line				 = 0x02,
	shape_triangle_plain_line			 = 0x03,
	shape_star_plain_line				 = 0x04,
	shape_circle_dotted_line			 = 0x05,
	shape_square_dotted_line			 = 0x06,
	shape_triangle_dotted_line			 = 0x07,
	shape_star_dotted_line				 = 0x08,
	number_0_zero						 = 0x09,
	number_1_one						 = 0x0A,
	number_2_two						 = 0x0B,
	number_3_three						 = 0x0C,
	number_4_four						 = 0x0D,
	number_5_five						 = 0x0E,
	number_6_six						 = 0x0F,
	number_7_seven						 = 0x10,
	number_8_eight						 = 0x11,
	number_9_nine						 = 0x12,
	number_10_ten						 = 0x13,
	color_red							 = 0x14,
	color_blue							 = 0x15,
	color_green							 = 0x16,
	color_yellow						 = 0x17,
	color_black							 = 0x18,
	color_white							 = 0x19,
	activity_magic_objects				 = 0x1A,
	activity_dance_freeze				 = 0x1B,
	activity_hide_and_seek				 = 0x1C,
	activity_music_colored_board		 = 0x1D,
	activity_music_quest				 = 0x1E,
	activity_color_quest				 = 0x1F,
	activity_colors_and_sounds			 = 0x20,
	activity_super_simon				 = 0x21,
	remote_colored_arrows				 = 0x22,
	remote_standard						 = 0x23,
	reinforcer_1_green_spin				 = 0x24,
	reinforcer_2_violet_green_blink_spin = 0x25,
	reinforcer_3_fire_static			 = 0x26,
	reinforcer_4_glitters_static		 = 0x27,
	reinforcer_5_rainbow_static			 = 0x28,
	emotion_happiness_child				 = 0x29,
	emotion_sadness_child				 = 0x2A,
	emotion_anger_child					 = 0x2B,
	emotion_fear_child					 = 0x2C,
	emotion_disgust_child				 = 0x2D,
	emotion_happiness_leka				 = 0x2E,
	emotion_sadness_leka				 = 0x2F,
	emotion_anger_leka					 = 0x30,
	emotion_fear_leka					 = 0x31,
	emotion_disgust_leka				 = 0x32,
	None
};

class RFIDUtils
{
  public:
	explicit RFIDUtils(rtos::EventFlags &event_flags)
		: _event_flags(event_flags),
		  rfid_serial {mbed_serial},
		  corerfid {rfid_serial, thread_rfid_action, rfid_event_queue},
		  rfidkit {corerfid} {};

	void initialize();
	void registerEventQueue(events::EventQueue &event_queue);

	auto getTag() -> Tag;

  private:
	void checkNewTag();

	EventQueue rfid_event_queue;
	rtos::Thread thread_rfid_action;
	rtos::EventFlags &_event_flags;

	mbed::BufferedSerial mbed_serial = {RFID_UART_TX, RFID_UART_RX, 57600};
	CoreBufferedSerial rfid_serial;
	CoreCR95HF corerfid;
	RFIDKit rfidkit;

	leka::rfid::Tag tag_container = {};
	Tag tag_id					  = Tag::None;
};

}	// namespace leka

#endif	 // _LEKA_OS_RFID_UTILS_H_
