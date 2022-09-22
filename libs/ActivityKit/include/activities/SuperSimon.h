// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "Color.h"
#include "Number.h"
#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class SuperSimon : public interface::Activity
{
  public:
	explicit SuperSimon(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit,
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

	static constexpr uint8_t kNumberOfColors = 6 * 2;
	static constexpr uint8_t kRoundsNumber	 = 5;

	uint8_t _expected_color_index = 0;
	uint8_t _current_round		  = 0;
	uint8_t _last_round			  = 5;
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<std::optional<Color>, kRoundsNumber> _expected_colors = {};
	std::array<Color, kNumberOfColors> _colors = {Color::blue,	 Color::blue,	Color::green,  Color::green,
												  Color::red,	 Color::red,	Color::purple, Color::purple,
												  Color::orange, Color::orange, Color::yellow, Color::yellow};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
