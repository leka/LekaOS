// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Disgusted.h"

#include "MathUtils.h"

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

auto Disgusted::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {50};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Disgusted::stage1()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
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
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColorAtIndex(0, green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, green_sick);
		_step++;
	} else {
		_stage++;
	}
}

void Disgusted::stage3()
{
	if (auto pos = mapStep(_step); pos != 0.F) {
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
