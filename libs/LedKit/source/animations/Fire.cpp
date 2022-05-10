// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Fire.h"

namespace leka::led::animation {

void Fire::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Fire::isRunning() -> bool
{
	return _running;
}

void Fire::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step			 = 0;
	_stage			 = 0;
	_wave_tail_index = 0;
	_running		 = true;
}

void Fire::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Fire::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	if (auto kLastStage = 150; _stage < kLastStage) {
		stageWaves();
	} else {
		turnLedBlack();
		_running = false;
	}
	_belt->show();
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
			_belt->setColorAtIndex(i, color);
		} else {
			RGB color = getFireColor(i - _wave_tail_index);
			_belt->setColorAtIndex(i - kNumberOfLedsBelt, color);
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
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
