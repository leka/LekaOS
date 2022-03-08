// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingRed.h"

#include "MathUtils.h"

namespace leka::led::animation {

void LoadingRed::start()
{
	turnLedBlack();
}

void LoadingRed::stop()
{
	turnLedBlack();
}

void LoadingRed::run()
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
		case 8:
			stage8();
			break;
		default:
			break;
	}
	_belt.show();
}

auto LoadingRed::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {60};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void LoadingRed::stage1()
{
	increaseBrightness();
}

void LoadingRed::stage2()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingRed::stage3()
{
	increaseBrightness();
}

void LoadingRed::stage4()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingRed::stage5()
{
	increaseBrightness();
}

void LoadingRed::stage6()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingRed::stage7()
{
	static constexpr auto kTreshold = 0.4F;
	increaseBrightness();
}

void LoadingRed::stage8()
{
	decreaseBrightness(0.F);
}

void LoadingRed::increaseBrightness()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingRed::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingRed::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
