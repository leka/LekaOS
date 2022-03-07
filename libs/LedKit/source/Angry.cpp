// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Angry.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Angry::start()
{
	turnLedBlack();
}

void Angry::stop()
{
	turnLedBlack();
}

void Angry::run()
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
		case 7:
			stage7();
			break;
		default:
			break;
	}
	_belt.show();
}

auto Angry::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {60};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Angry::stage1()
{
	increaseBrightness(1.F);
}

void Angry::stage2()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void Angry::stage3()
{
	increaseBrightness(1.F);
}

void Angry::stage4()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void Angry::stage5()
{
	increaseBrightness(1.F);
}

void Angry::stage6()
{
	if (auto pos = mapStep(_step); pos != 0.F) {
		_step--;
	} else {
		_step = 60;
		_stage++;
	}
}

void Angry::stage7()
{
	decreaseBrightness(0.F);
}

void Angry::increaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Angry::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Angry::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
