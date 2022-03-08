// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingOrange.h"

#include "MathUtils.h"

namespace leka::led::animation {

void LoadingOrange::start()
{
	turnLedBlack();
}

void LoadingOrange::stop()
{
	turnLedBlack();
}

void LoadingOrange::run()
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

auto LoadingOrange::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {40};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void LoadingOrange::stage1()
{
	increaseBrightness();
}

void LoadingOrange::stage2()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingOrange::stage3()
{
	increaseBrightness();
}

void LoadingOrange::stage4()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingOrange::stage5()
{
	increaseBrightness();
}

void LoadingOrange::stage6()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingOrange::stage7()
{
	increaseBrightness();
}

void LoadingOrange::stage8()
{
	decreaseBrightness(0.F);
}

void LoadingOrange::increaseBrightness()
{
	static constexpr auto orange = RGB {0xFF, 0xA5, 0x00};
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, orange, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingOrange::decreaseBrightness(float treshold)
{
	static constexpr auto orange = RGB {0xFF, 0xA5, 0x00};
	if (auto pos = mapStep(_step); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, orange, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingOrange::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
