// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Happy.h"

#include "MathUtils.h"

namespace leka::led::animation {

static constexpr auto kNumberOfLedsBelt = uint8_t {20};

void Happy::start()
{
	turnLedBlack();
}

void Happy::stop()
{
	turnLedBlack();
}

void Happy::run()
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
		case 4:
			stage4();
			break;
		case 5:
			stage5();
			break;
		default:
			break;
	}
	_belt.show();
}

auto Happy::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {15};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Happy::stage1()
{
	static constexpr auto pink_happy = RGB {255, 98, 98};
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, pink_happy, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Happy::stage2()
{
	static constexpr auto pink_happy = RGB {255, 98, 98};
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColorAtIndex(0, pink_happy);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, pink_happy);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Happy::stage3()
{
	static constexpr auto pink_happy = RGB {255, 98, 98};
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(pink_happy, RGB::pure_red, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Happy::stage4()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColorAtIndex(0, RGB::pure_red);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, RGB::pure_red);
		_step++;
	} else {
		_stage++;
	}
}

void Happy::stage5()
{
	if (auto pos = mapStep(_step); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step--;
	} else {
		_stage++;
	}
}

void Happy::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
