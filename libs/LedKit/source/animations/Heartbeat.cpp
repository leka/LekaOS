// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Heartbeat.h"

namespace leka::led::animation {

void Heartbeat::setLeds(interface::LED &ears, interface::LED &belt)
{
	_belt = &belt;
	_ears = &ears;
}

auto Heartbeat::isRunning() -> bool
{
	return _running;
}

void Heartbeat::start()
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

void Heartbeat::stop()
{
	if (_belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Heartbeat::run()
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
		case 3:
			stage3();
			break;

		case 4:
			stage4();
			break;

		default:
			_running = false;	//
			break;
	}

	_belt->show();
}

void Heartbeat::stage0()
{
	static constexpr auto max = uint8_t {5};
	increaseBrightness(max, RGB::pure_red);
}

void Heartbeat::stage1()
{
	static constexpr auto max = uint8_t {5};
	decreaseBrightness(max, RGB::pure_red);
}

void Heartbeat::stage2()
{
	static constexpr auto max = uint8_t {30};
	increaseBrightness(max, RGB::pure_red);
}

void Heartbeat::stage3()
{
	static constexpr auto max = uint8_t {30};
	decreaseBrightness(max, RGB::pure_red);
}

void Heartbeat::stage4()
{
	static const uint8_t kMaxInputValue = {40};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		++_step;

	} else {
		if (_turn < 10) {
			_step  = 0;
			_stage = 0;
			_turn++;
		} else {
			++_stage;
		}
	}
}

void Heartbeat::increaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = uint8_t {max};
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

void Heartbeat::decreaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = uint8_t {max};
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

void Heartbeat::turnLedBlack()
{
	_belt->setColor(RGB::black);
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
