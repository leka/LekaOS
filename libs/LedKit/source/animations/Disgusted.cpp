// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Disgusted.h"

namespace leka::led::animation {

static constexpr auto kNumberOfLedsBelt = uint8_t {20};

static constexpr auto green_sick = RGB {30, 255, 0};

void Disgusted::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Disgusted::isRunning() -> bool
{
	return _running;
}

void Disgusted::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Disgusted::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Disgusted::run()
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
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void Disgusted::stage0()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Disgusted::stage1()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColorAtIndex(0, green_sick);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, green_sick);
		_step++;
	} else {
		_stage++;
	}
}

void Disgusted::stage2()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step--;
	} else {
		_stage++;
	}
}

void Disgusted::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
