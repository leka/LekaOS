// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Disgusted.h"

namespace leka::led::animation {

static constexpr auto kNumberOfLedsBelt = uint8_t {20};

static constexpr auto green_sick = RGB {30, 255, 0};

void Disgusted::start()
{
	turnLedBlack();
}

void Disgusted::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 1;
}

void Disgusted::run()
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

void Disgusted::stage1()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Disgusted::stage2()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt.setColorAtIndex(0, green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, green_sick);
		_step++;
	} else {
		_stage++;
	}
}

void Disgusted::stage3()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step--;
	} else {
		_stage++;
	}
}

void Disgusted::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
