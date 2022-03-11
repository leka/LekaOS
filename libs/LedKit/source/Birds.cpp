// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Birds.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Birds::start()
{
	turnLedBlack();
}

void Birds::stop()
{
	turnLedBlack();
	_step			   = 0;
	_stage			   = 1;
	_wave_tail_index_1 = 0;
	_wave_tail_index_2 = 6;
	_wave_tail_index_3 = 13;
}

void Birds::run()
{
	static constexpr auto kLastStage = uint8_t {100};
	if (_stage == 1) {
		stage1();
	} else if (_stage <= kLastStage) {
		stageWaves();
	} else {
		_belt.setColor(RGB::black);
	}
	_belt.show();
}

void Birds::stage1()
{
	_belt.setColor(RGB::pure_red);
	_stage++;
}

void Birds::stageWaves()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColor(RGB::pure_red);
	moveWave(_wave_tail_index_1);
	moveWave(_wave_tail_index_2);
	moveWave(_wave_tail_index_3);
	_stage++;
}

void Birds::moveWave(uint8_t &wave_tail_index)
{
	static constexpr auto orange			= RGB {255, 80, 0};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (wave_tail_index == kNumberOfLedsBelt) {
		wave_tail_index = 0;
	}
	for (auto i = wave_tail_index; i < wave_tail_index + 3; i++) {
		if (i < kNumberOfLedsBelt) {
			_belt.setColorAtIndex(i, orange);
		}
	}
	++wave_tail_index;
}

void Birds::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
