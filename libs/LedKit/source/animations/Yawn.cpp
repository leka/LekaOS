// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Yawn.h"

namespace leka::led::animation {

void Yawn::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Yawn::isRunning() -> bool
{
	return _running;
}

void Yawn::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Yawn::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Yawn::run()
{
	if (_ears == nullptr || _belt == nullptr) {
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
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void Yawn::stage0()
{
	static constexpr auto kInputMaxValue1 = uint8_t {18};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Yawn::stage1()
{
	static constexpr auto kInputMaxValue1 = uint8_t {18};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Yawn::stage2()
{
	static constexpr auto kInputMaxValue1 = uint8_t {18};
	static constexpr auto kInputMaxValue2 = uint8_t {5};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue2); pos != 1.F) {
		++_step;
	} else {
		_step = kInputMaxValue1;
		++_stage;
	}
}

void Yawn::stage3()
{
	static constexpr auto kInputMaxValue1 = uint8_t {18};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		--_step;
	} else {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		++_stage;
	}
}

void Yawn::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
