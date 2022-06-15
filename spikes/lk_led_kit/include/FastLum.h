// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "LEDAnimation.h"

namespace leka::led::animation {

class FastLum : public interface::LEDAnimation
{
  public:
	explicit FastLum() = default;

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;
	auto isRunning() -> bool final;

  private:
	interface::LED *_ears = nullptr;
	interface::LED *_belt = nullptr;
	bool _running		  = false;
	uint8_t _step		  = 0;
	uint8_t _stage		  = 0;
	void pureRedFor80BlackFor40Back();
	void pureRedFor80BlackFor40Front();
	void animation1();
	void animation2();
	void animation3Part1();
	void animation3Part2();
	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
