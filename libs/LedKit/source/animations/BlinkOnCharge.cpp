// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "BlinkOnCharge.h"

namespace leka::led::animation {

void BlinkOnCharge::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto BlinkOnCharge::isRunning() -> bool
{
	return _running;
}

void BlinkOnCharge::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void BlinkOnCharge::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void BlinkOnCharge::run()
{
	const auto kStageDurationEarsOn = uint8_t {20};

	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	if (_stage < kStageDurationEarsOn) {
		_ears->setColor(RGB::pure_green);
		_ears->show();
	} else {
		turnLedBlack();
	}

	++_stage;
}

void BlinkOnCharge::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
