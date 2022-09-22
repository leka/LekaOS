// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <optional>
#include <random>

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "include/Color.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class LedColorRecognition : public interface::Activity
{
  public:
	explicit LedColorRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit,
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

	static constexpr uint8_t kRoundsNumber		= 10;
	static constexpr uint8_t kSizeOfColorsTable = 6 * 2;

	uint8_t _current_round = 0;
	std::optional<Color> _current_color {};
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<Color, kSizeOfColorsTable> _colors = {Color::purple, Color::purple, Color::blue,	  Color::blue,
													 Color::green,	Color::green,  Color::yellow, Color::yellow,
													 Color::orange, Color::orange, Color::red,	  Color::red};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
