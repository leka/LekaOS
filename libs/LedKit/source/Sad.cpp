// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Sad.h"

namespace leka::led::animation {

void Sad::start()
{
	turnLedBlack();
}

void Sad::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void Sad::run()
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

void Sad::stage0()
{
	increaseBrightness();
}

void Sad::stage1()
{
	static constexpr auto kTreshold = 0.5F;
	decreaseBrightness(kTreshold);
}

void Sad::stage2()
{
	increaseBrightness();
}

void Sad::stage3()
{
	static constexpr auto kTreshold = 0.5F;
	decreaseBrightness(kTreshold);
}

void Sad::stage4()
{
	increaseBrightness();
}

void Sad::stage5()
{
	static constexpr auto kTreshold = 0.5F;
	decreaseBrightness(kTreshold);
}

void Sad::stage6()
{
	increaseBrightness();
}

void Sad::stage7()
{
	decreaseBrightness(0.F);
}

void Sad::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {52};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Sad::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {52};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Sad::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
