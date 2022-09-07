// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class ChooseReinforcer : public interface::Activity
{
  public:
	explicit ChooseReinforcer(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start() final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	std::function<void(const MagicCard &)> _backup_callback {};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
