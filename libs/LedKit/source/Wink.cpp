// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Wink.h"

namespace leka::led::animation {

void Wink::start()
{
	turnLedBlack();
}

void Wink::stop()
{
	turnLedBlack();
}

void Wink::run()
{
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
		default:
			break;
	}
	_belt.show();
}

void Wink::stage1()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	static constexpr auto kInputMaxValue1	= uint8_t {20};
	static constexpr auto pink				= RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 1.F) {
		auto color = ColorKit::colorGradient(RGB::black, pink, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Wink::stage2()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	static constexpr auto kInputMaxValue2	= uint8_t {40};
	static constexpr auto pink				= RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue2); pos != 1.F) {
		_belt.setColorAtIndex(0, pink);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, pink);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Wink::stage3()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	static constexpr auto kInputMaxValue1	= uint8_t {20};
	static constexpr auto pink				= RGB {255, 98, 98};
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue1); pos != 1.F) {
		auto color = ColorKit::colorGradient(pink, RGB::black, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		++_step;
	} else {
		++_stage;
	}
}

void Wink::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
