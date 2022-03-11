// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Birds : public interface::LEDAnimation
{
  public:
	explicit Birds(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {};

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step			   = 0;
	uint8_t _stage			   = 1;
	uint8_t _wave_tail_index_1 = 0;
	uint8_t _wave_tail_index_2 = 4;
	uint8_t _wave_tail_index_3 = 9;

	void stage1();
	void stageWaves();
	void moveWave(uint8_t &wave_tail_index);

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
