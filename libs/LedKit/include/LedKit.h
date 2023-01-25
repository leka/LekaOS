// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/HighResClock.h"

#include "ColorKit.h"
#include "LEDAnimation.h"
#include "interface/libs/EventLoop.h"
#include "interface/libs/LedKit.h"

namespace leka {
class LedKit : public interface::LedKit
{
  public:
	static constexpr auto kNumberOfLedsEars = 2;
	static constexpr auto kNumberOfLedsBelt = 20;

	LedKit(interface::EventLoop &event_loop, interface::LED &ears, interface::LED &belt)
		: _event_loop(event_loop), _ears(ears), _belt(belt) {};

	void init() final;
	void start(interface::LEDAnimation *animation) final;
	void run() final;
	void stop() final;

  private:
	interface::EventLoop &_event_loop;

	interface::LED &_ears;
	interface::LED &_belt;

	interface::LEDAnimation *_animation = nullptr;
};

}	// namespace leka
