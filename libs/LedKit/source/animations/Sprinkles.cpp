// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Sprinkles.h"

namespace leka::led::animation {

void Sprinkles::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Sprinkles::isRunning() -> bool
{
	return _running;
}

void Sprinkles::start()
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

void Sprinkles::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Sprinkles::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	static constexpr auto kLastStage = uint8_t {150};
	auto is_wave_moving				 = [this] { return (_stage % 2) == 0; };
	if (is_wave_moving() && _stage <= kLastStage) {
		_belt->setColor(RGB::black);
		moveWave(_wave_tail_index);
		++_stage;
	} else if (_stage <= kLastStage) {
		++_stage;
	} else {
		_running = false;
		turnLedBlack();
	}
	_belt->show();
}

void Sprinkles::moveWave(uint8_t &wave_tail_index)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};

	if (wave_tail_index == kNumberOfLedsBelt) {
		wave_tail_index = 0;
	}
	for (auto i = wave_tail_index; i < kNumberOfLedsBelt + wave_tail_index; i += 3) {
		if (i < kNumberOfLedsBelt) {
			_belt->setColorAtIndex(i, getSprinklesColor());
		} else {
			_belt->setColorAtIndex(i - kNumberOfLedsBelt, getSprinklesColor());
		}
	}
	++wave_tail_index;
}

auto Sprinkles::getSprinklesColor() const -> RGB
{
	auto get_random_color = []() { return std::rand() / ((RAND_MAX + 1U) / 255); };
	auto red			  = get_random_color();
	auto green			  = get_random_color();
	auto blue			  = get_random_color();
	return RGB {static_cast<uint8_t>(red), static_cast<uint8_t>(green), static_cast<uint8_t>(blue)};
}

void Sprinkles::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
