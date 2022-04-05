// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Sick : public interface::LEDAnimation
{
  public:
	explicit Sick(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {}

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step			= 0;
	uint8_t _stage			= 0;
	uint8_t _sneeze_index_1 = 0;
	uint8_t _sneeze_index_2 = 0;
	uint8_t _sneeze_index_3 = 0;

	void stage0();
	void stage1and4();
	void stage2and5();
	void stage3and6();
	void stage7();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
