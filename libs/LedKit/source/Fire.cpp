// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Fire.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Fire::start()
{
	turnLedBlack();
}

void Fire::stop()
{
	turnLedBlack();
	_step			 = 0;
	_stage			 = 0;
	_wave_tail_index = 0;
}

void Fire::run()
{
	if (auto kLastStage = 255; _stage < kLastStage) {
		stageWaves();
	} else {
		_belt.setColor(RGB::black);
	}
	_belt.show();
}

void Fire::stageWaves()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};

	if (_wave_tail_index == kNumberOfLedsBelt) {
		_wave_tail_index = 0;
	}
	for (auto i = _wave_tail_index; i < _wave_tail_index + kNumberOfLedsBelt; i++) {
		if (i < kNumberOfLedsBelt) {
			RGB color = getFireColor(i - _wave_tail_index);
			_belt.setColorAtIndex(i, color);
		} else {
			RGB color = getFireColor(i - _wave_tail_index);
			_belt.setColorAtIndex(i - kNumberOfLedsBelt, color);
		}
	}
	++_wave_tail_index;
	++_stage;
}

auto Fire::getFireColor(uint8_t index) const -> RGB
{
	auto position = static_cast<float>(index % 10) / 10;
	RGB color	  = ColorKit::colorGradient(RGB::pure_red, RGB::yellow, position);
	return color;
}

void Fire::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
