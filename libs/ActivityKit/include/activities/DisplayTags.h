// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "Activity.h"
#include "RFIDKit.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class DisplayTags : public interface::Activity
{
  public:
	explicit DisplayTags(RFIDKit &rfidkit, interface::VideoKit &videokit) : _rfidkit(rfidkit), _videokit(videokit) {};

	void start() final;
	void stop() final;

  private:
	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	std::array<char, 32> path = {};
	std::function<void(MagicCard &)> _backup_callback {};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
