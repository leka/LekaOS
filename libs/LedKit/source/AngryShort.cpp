// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "AngryShort.h"

#include "MathUtils.h"

namespace leka::led::animation {

void AngryShort::start()
{
	turnLedBlack();
}

void AngryShort::stop()
{
	turnLedBlack();
}

void AngryShort::run()
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
		case 6:
			stage6();
			break;
		default:
			break;
	}
	_belt.show();
}

auto AngryShort::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {47};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void AngryShort::stage1()
{
	increaseBrightness(1.F);
}

void AngryShort::stage2()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AngryShort::stage3()
{
	increaseBrightness(1.F);
}

void AngryShort::stage4()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AngryShort::stage5()
{
	increaseBrightness(1.F);
}

void AngryShort::stage6()
{
	decreaseBrightness(0.F);
}

void AngryShort::increaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void AngryShort::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos >= treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void AngryShort::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
