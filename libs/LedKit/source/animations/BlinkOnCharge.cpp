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
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			increaseBrightness(20, light_white);
			break;
		case 1:
			decreaseBrightness(20, light_white);
			break;

		case 2:
			static constexpr auto kMaxEarsOffDuration = uint8_t {100};
			if (_step < kMaxEarsOffDuration) {
				turnLedBlack();
				++_step;
			} else {
				_step = 0;
				++_stage;
			}
			break;

		default:
			_stage = 0;
			break;
	}
	_ears->show();
}

void BlinkOnCharge::increaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = max;
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color_gradient = ColorKit::colorGradient(RGB::black, color, pos);
		_ears->setColor(color_gradient);
		++_step;

	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkOnCharge::decreaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = max;
	if (auto pos = utils::normalizeStep(kMaxInputValue - _step, max); pos != 0) {
		RGB color_gradient = ColorKit::colorGradient(RGB::black, color, pos);
		_ears->setColor(color_gradient);
		++_step;

	} else {
		_step = 0;
		++_stage;
	}
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
