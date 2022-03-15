// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "AfraidBlue.h"

#include "MathUtils.h"

namespace leka::led::animation {

void AfraidBlue::start()
{
	turnLedBlack();
}

void AfraidBlue::stop()
{
	turnLedBlack();
}

void AfraidBlue::run()
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
	_ears.show();
	_belt.show();
}

auto AfraidBlue::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {30};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void AfraidBlue::stage1()
{
	increaseBrightness();
}

void AfraidBlue::stage2()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AfraidBlue::stage3()
{
	increaseBrightness();
}

void AfraidBlue::stage4()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AfraidBlue::stage5()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt.setColor(color);
		_ears.setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void AfraidBlue::stage6()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColor(RGB {0, 128, 255});
		_ears.setColor(RGB {0, 128, 255});
		_step++;
	} else {
		_stage++;
	}
}

void AfraidBlue::stage7()
{
	decreaseBrightness(0.F);
}

void AfraidBlue::increaseBrightness()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt.setColor(color);
		_ears.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void AfraidBlue::decreaseBrightness(float treshold)
{
	if (auto pos = mapStep(_step); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt.setColor(color);
		_ears.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void AfraidBlue::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
