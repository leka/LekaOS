// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Breath.h"

using namespace std::chrono;

namespace leka::led::animation {

void Breath::setLeds(interface::LED &ears, interface::LED &belt)
{
	_belt = &belt;
}

auto Breath::isRunning() -> bool
{
	return _running;
}

void Breath::start()
{
	if (_belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_turn	 = 0;
	_running = true;
}

void Breath::stop()
{
	if (_belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Breath::run()
{
	if (_belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			stage0();
			break;
		case 1:
			stage1();
			break;

		case 2:
			stage2();
			break;

		default:
			_running = false;	//
			break;
	}

	_belt->show();
}

void Breath::stage0()
{
	static constexpr auto max = uint8_t {60};
	increaseBrightness(max, RGB::pure_blue);
}

void Breath::stage1()
{
	static constexpr auto max = uint8_t {60};
	decreaseBrightness(max, RGB::pure_blue);
}

void Breath::stage2()
{
	static const uint8_t kMaxInputValue = {10};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_step++;
	}

	if (_turn < 10) {
		_turn++;
		_stage = 0;

	} else {
		_stage++;
	}
}

void Breath::increaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = max;
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color_gradient = ColorKit::colorGradient(RGB::black, color, pos);
		_belt->setColor(color_gradient);
		_belt->show();
		_step++;

	} else {
		_step = 0;
		_stage++;
	}
}

void Breath::decreaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = max;
	if (auto pos = utils::normalizeStep(kMaxInputValue - _step, max); pos != 0) {
		RGB color_gradient = ColorKit::colorGradient(RGB::black, color, pos);
		_belt->setColor(color_gradient);
		_belt->show();
		_step++;

	} else {
		_step = 0;
		_stage++;
	}
}

void Breath::turnLedBlack()
{
	_belt->setColor(RGB::black);
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
