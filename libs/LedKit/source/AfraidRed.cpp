// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "AfraidRed.h"

namespace leka::led::animation {

void AfraidRed::start()
{
	turnLedBlack();
}

void AfraidRed::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 1;
}

void AfraidRed::run()
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

void AfraidRed::stage1()
{
	increaseBrightness();
}

void AfraidRed::stage2()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AfraidRed::stage3()
{
	increaseBrightness();
}

void AfraidRed::stage4()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AfraidRed::stage5()
{
	static constexpr auto kMaxInputValue = uint8_t {34};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void AfraidRed::stage6()
{
	static constexpr auto kMaxInputValue = uint8_t {34};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt.setColor(RGB::pure_red);
		_step++;
	} else {
		_stage++;
	}
}

void AfraidRed::stage7()
{
	decreaseBrightness(0.F);
}

void AfraidRed::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {34};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void AfraidRed::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {34};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos >= treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void AfraidRed::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
