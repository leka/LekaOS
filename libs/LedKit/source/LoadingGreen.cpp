// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingGreen.h"

#include "MathUtils.h"

namespace leka::led::animation {

void LoadingGreen::start()
{
	turnLedBlack();
}

void LoadingGreen::stop()
{
	turnLedBlack();
}

void LoadingGreen::run()
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
		case 11:
			stage11();
			break;
		case 12:
			stage12();
			break;
		default:
			break;
	}
	_belt.show();
}

auto LoadingGreen::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {20};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void LoadingGreen::stage1()
{
	increaseBrightness();
}

void LoadingGreen::stage2()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage3()
{
	increaseBrightness();
}

void LoadingGreen::stage4()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage5()
{
	increaseBrightness();
}

void LoadingGreen::stage6()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage7()
{
	increaseBrightness();
}

void LoadingGreen::stage8()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage9()
{
	increaseBrightness();
}

void LoadingGreen::stage10()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage11()
{
	increaseBrightness();
}

void LoadingGreen::stage12()
{
	decreaseBrightness(0.F);
}

void LoadingGreen::increaseBrightness()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_green, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingGreen::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_green, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingGreen::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
