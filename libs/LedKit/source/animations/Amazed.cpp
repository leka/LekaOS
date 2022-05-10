// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Amazed.h"

namespace leka::led::animation {

void Amazed::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Amazed::isRunning() -> bool
{
	return _running;
}

void Amazed::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Amazed::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Amazed::run()
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

void Amazed::stage0()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Amazed::stage1()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Amazed::stage2()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColor(RGB::white);
		_step++;
	} else {
		_stage++;
	}
}

void Amazed::stage3()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Amazed::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
