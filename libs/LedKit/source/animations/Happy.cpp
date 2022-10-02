// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Happy.h"

namespace leka::led::animation {

static constexpr auto kNumberOfLedsBelt = uint8_t {20};

void Happy::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Happy::isRunning() -> bool
{
	return _running;
}

void Happy::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Happy::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Happy::run()
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
		case 4:
			stage4();
			break;
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void Happy::stage0()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	static constexpr auto pink_happy	 = RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, pink_happy, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Happy::stage1()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	static constexpr auto pink_happy	 = RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColorAtIndex(0, pink_happy);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, pink_happy);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Happy::stage2()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	static constexpr auto pink_happy	 = RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(pink_happy, RGB::pure_red, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Happy::stage3()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColorAtIndex(0, RGB::pure_red);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, RGB::pure_red);
		_step++;
	} else {
		_stage++;
	}
}

void Happy::stage4()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step--;
	} else {
		_stage++;
	}
}

void Happy::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
