// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "AngryShort.h"

namespace leka::led::animation {

void AngryShort::start()
{
	turnLedBlack();
}

void AngryShort::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void AngryShort::run()
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
		default:
			break;
	}
	_belt.show();
}

void AngryShort::stage0()
{
	increaseBrightness(1.F);
}

void AngryShort::stage1()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AngryShort::stage2()
{
	increaseBrightness(1.F);
}

void AngryShort::stage3()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AngryShort::stage4()
{
	increaseBrightness(1.F);
}

void AngryShort::stage5()
{
	decreaseBrightness(0.F);
}

void AngryShort::increaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {47};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void AngryShort::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {47};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos >= treshold) {
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
