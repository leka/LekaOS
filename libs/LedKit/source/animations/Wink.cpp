// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Wink.h"

namespace leka::led::animation {

void Wink::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Wink::isRunning() -> bool
{
	return _running;
}

void Wink::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Wink::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Wink::run()
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

void Wink::stage0()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	static constexpr auto kInputMaxValue1	= uint8_t {20};
	static constexpr auto pink				= RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 1.F) {
		auto color = ColorKit::colorGradient(RGB::black, pink, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Wink::stage1()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	static constexpr auto kInputMaxValue2	= uint8_t {40};
	static constexpr auto pink				= RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue2); pos != 1.F) {
		_belt->setColorAtIndex(0, pink);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, pink);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Wink::stage2()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	static constexpr auto kInputMaxValue1	= uint8_t {20};
	static constexpr auto pink				= RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 1.F) {
		auto color = ColorKit::colorGradient(pink, RGB::black, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		++_step;
	} else {
		++_stage;
	}
}

void Wink::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
