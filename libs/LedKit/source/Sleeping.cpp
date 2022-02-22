// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Sleeping.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Sleeping::start()
{
	turnLedBlack();
}

void Sleeping::stop()
{
	turnLedBlack();
}

void Sleeping::run()
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

auto Sleeping::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {45};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Sleeping::stage1()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Sleeping::stage2()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = RGB::white;
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Sleeping::stage3()
{
	static constexpr auto kTreshold1 = 0.45F;
	decreaseBrightness(kTreshold1);
}

void Sleeping::stage4()
{
	static constexpr auto kTreshold2 = 0.9F;
	increaseBrightness(kTreshold2);
}

void Sleeping::stage5()
{
	static constexpr auto kTreshold3 = 0.45F;
	decreaseBrightness(kTreshold3);
}

void Sleeping::stage6()
{
	static constexpr auto kTreshold4 = 0.80F;
	increaseBrightness(kTreshold4);
}

void Sleeping::stage7()
{
	decreaseBrightness(0.F);
}

void Sleeping::increaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos < treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Sleeping::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Sleeping::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
