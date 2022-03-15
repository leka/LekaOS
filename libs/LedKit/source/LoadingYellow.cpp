// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingYellow.h"

#include "MathUtils.h"

namespace leka::led::animation {

void LoadingYellow::start()
{
	turnLedBlack();
}

void LoadingYellow::stop()
{
	turnLedBlack();
}

void LoadingYellow::run()
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
		case 9:
			stage9();
			break;
		case 10:
			stage10();
			break;
		default:
			break;
	}
	_belt.show();
}

auto LoadingYellow::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {25};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void LoadingYellow::stage1()
{
	increaseBrightness();
}

void LoadingYellow::stage2()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage3()
{
	increaseBrightness();
}

void LoadingYellow::stage4()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage5()
{
	increaseBrightness();
}

void LoadingYellow::stage6()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage7()
{
	increaseBrightness();
}

void LoadingYellow::stage8()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage9()
{
	increaseBrightness();
}

void LoadingYellow::stage10()
{
	decreaseBrightness(0.F);
}

void LoadingYellow::increaseBrightness()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::yellow, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingYellow::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::yellow, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingYellow::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
