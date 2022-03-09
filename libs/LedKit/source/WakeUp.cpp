// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "WakeUp.h"

#include "MathUtils.h"

namespace leka::led::animation {

void WakeUp::start()
{
	turnLedBlack();
}

void WakeUp::stop()
{
	turnLedBlack();
}

void WakeUp::run()
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

auto WakeUp::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {25};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void WakeUp::stage0()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void WakeUp::stage1()
{
	static constexpr auto kTreshold1 = 0.80F;
	increaseBrightness(kTreshold1);
}

void WakeUp::stage2()
{
	static constexpr auto kTreshold2 = 0.45F;
	decreaseBrightness(kTreshold2);
}

void WakeUp::stage3()
{
	static constexpr auto kTreshold3 = 0.9F;
	increaseBrightness(kTreshold3);
}

void WakeUp::stage4()
{
	static constexpr auto kTreshold4 = 0.5F;
	decreaseBrightness(kTreshold4);
}

void WakeUp::stage5()
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

void WakeUp::stage6()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = RGB::white;
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void WakeUp::stage7()
{
	decreaseBrightness(0.F);
}

void WakeUp::increaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos < treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void WakeUp::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void WakeUp::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
