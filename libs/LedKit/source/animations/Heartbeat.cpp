// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Heartbeat.h"

namespace leka::led::animation {

void Heartbeat::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Heartbeat::isRunning() -> bool
{
	return _running;
}

void Heartbeat::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Heartbeat::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Heartbeat::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
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
			_running = false;
			break;
	}
	_ears->show();
	_belt->show();
}

void Heartbeat::stage1()
{
	static constexpr auto kMaxInputValue = uint8_t {10};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {255, 0, 0}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Heartbeat::stage2()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void Heartbeat::stage3()
{
	static constexpr auto kMaxInputValue = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {255, 0, 0}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Heartbeat::stage4()
{
	static constexpr auto kTreshold = 0.2F;
	decreaseBrightness(kTreshold);
}

void AfraidBlue::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
