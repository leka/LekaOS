// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// #include <cstdlib>
#include "rtos/ThisThread.h"

#include "BehaviorKit.h"
#include "CoreEventFlags.h"

namespace leka {

class Screensaver
{
  public:
	explicit Screensaver(BehaviorKit &behaviorkit) : _behaviorkit(behaviorkit) {};

	void start();
	void stop();
	void onScreensaverEnded(std::function<void()> const &callback);

	// struct flags {
	// 	static constexpr uint32_t START_SCREENSAVER_FLAG = (1UL << 1);
	// 	static constexpr uint32_t STOP_SCREENSAVER_FLAG	 = (1UL << 2);
	// };

  private:
	BehaviorKit &_behaviorkit;
	// CoreEventFlags _event_flags {};

	std::function<void()> _on_screensaver_ended {};
};
}	// namespace leka
