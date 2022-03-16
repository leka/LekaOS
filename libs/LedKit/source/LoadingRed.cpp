// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingRed.h"

namespace leka::led::animation {

void LoadingRed::start()
{
	turnLedBlack();
}

void LoadingRed::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void LoadingRed::run()
{
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

void LoadingRed::stage0()
{
	increaseBrightness();
}

void LoadingRed::stage1()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingRed::stage2()
{
	increaseBrightness();
}

void LoadingRed::stage3()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingRed::stage4()
{
	increaseBrightness();
}

void LoadingRed::stage5()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingRed::stage6()
{
	static constexpr auto kTreshold = 0.4F;
	increaseBrightness();
}

void LoadingRed::stage7()
{
	decreaseBrightness(0.F);
}

void LoadingRed::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {100};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingRed::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {100};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != treshold) {
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
