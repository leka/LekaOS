// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <optional>

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "include/Food.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class FoodRecognition : public interface::Activity
{
  public:
	explicit FoodRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start(const std::function<void()> &before_process_callback) final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	static constexpr uint8_t kScoreToWin	   = 10;
	static constexpr uint8_t kSizeOfFoodsTable = 9 * 2;

	uint8_t _current_round = 0;
	std::optional<Food> _current_food {};
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<Food, kSizeOfFoodsTable> _foods = {
		Food::apple,	Food::apple,	Food::banana,	  Food::banana,		Food::grapes, Food::grapes,
		Food::mushroom, Food::mushroom, Food::salad,	  Food::salad,		Food::carrot, Food::carrot,
		Food::cherry,	Food::cherry,	Food::strawberry, Food::strawberry, Food::potato, Food::potato,
	};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
