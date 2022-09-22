// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <optional>

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "include/Number.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class NumberRecognition : public interface::Activity
{
  public:
	explicit NumberRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start() final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	static constexpr uint8_t kRoundsNumber		= 10;
	static constexpr uint8_t kSizeOfNumberTable = 11 * 2;

	uint8_t _current_round = 0;
	std::optional<Number> _current_number {};
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<Number, kSizeOfNumberTable> _numbers = {
		Number::number_0, Number::number_1, Number::number_2, Number::number_3, Number::number_4,  Number::number_5,
		Number::number_6, Number::number_7, Number::number_8, Number::number_9, Number::number_10, Number::number_0,
		Number::number_1, Number::number_2, Number::number_3, Number::number_4, Number::number_5,  Number::number_6,
		Number::number_7, Number::number_8, Number::number_9, Number::number_10};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
