// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <optional>

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "include/Color.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class PictoColorRecognition : public interface::Activity
{
  public:
	explicit PictoColorRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start() final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	static constexpr uint8_t kRoundsNumber	   = 10;
	static constexpr uint8_t kSizeOfColorTable = 4 * 3;

	uint8_t _current_round = 0;
	std::optional<Color> _current_color {};
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<Color, kSizeOfColorTable> _colors = {Color::blue,   Color::blue,	 Color::blue,	Color::green,
													Color::green,  Color::green, Color::yellow, Color::yellow,
													Color::yellow, Color::red,	 Color::red,	Color::red};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
