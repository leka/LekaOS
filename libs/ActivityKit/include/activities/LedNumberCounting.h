// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "MathUtils.h"
#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class LedNumberCounting : public interface::Activity
{
  public:
	explicit LedNumberCounting(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit,
							   interface::LED &led)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit), _led(led) {};

	void start() final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;
	interface::LED &_led;

	static constexpr uint8_t kRoundsNumber		   = 10;
	static constexpr uint8_t kSizeOfColorsTable	   = 6;
	static constexpr uint8_t kSizeOfLedNumberTable = 12;
	static constexpr uint8_t kSizeOfLedIndexTable  = 7;

	static constexpr std::array<RGB, kSizeOfColorsTable> colors_table = {RGB::orange, RGB::pure_green, RGB::cyan,
																		 RGB::yellow, RGB::magenta,	   RGB::pure_red};

	uint8_t _current_round		   = 0;
	uint8_t _current_leds_number   = 0;
	MagicCard _expected_tag_number = MagicCard::none;

	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<uint8_t, kSizeOfLedNumberTable> _led_numbers = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
	std::array<uint8_t, kSizeOfLedIndexTable> _led_indexes	= {0, 3, 6, 9, 12, 15, 19};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
