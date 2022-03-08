// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "SadCry.h"

#include "LogKit.h"
#include "MathUtils.h"

namespace leka::led::animation {

void SadCry::start()
{
	turnLedBlack();
}

void SadCry::stop()
{
	turnLedBlack();
}

void SadCry::run()
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
		case 13:
			stage13();
			break;
		default:
			break;
	}
	log_debug("_step = %i", _step);
	log_debug("_stage = %i", _stage);
	_belt.show();
}

auto SadCry::mapStep(uint8_t step, uint8_t max_input_value) const -> float
{
	return utils::math::map(step, uint8_t {0}, max_input_value, 0.F, 1.F);
}

void SadCry::stage1()
{
	static constexpr auto kMaxInputValueFirstStages = uint8_t {90};
	if (auto pos = mapStep(_step, kMaxInputValueFirstStages); pos != 1.F) {
		RGB shade_of_blue = ColorKit::colorGradient(RGB::black, RGB::pure_blue, pos);
		_belt.setColor(shade_of_blue);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void SadCry::stage2()
{
	static constexpr auto kMaxInputValueFirstStages = uint8_t {90};
	if (auto pos = mapStep(_step, kMaxInputValueFirstStages); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void SadCry::stage3()
{
	static constexpr auto kNumberOfLedsBelt		   = uint8_t {20};
	static constexpr auto kMaxInputValueLastStages = uint8_t {30};

	static constexpr auto pink = RGB {255, 98, 98};

	if (auto pos = mapStep(_step, kMaxInputValueLastStages); pos != 1.F) {
		RGB shade_of_pink = ColorKit::colorGradient(RGB::pure_blue, pink, pos);
		_belt.setColorAtIndex(0, shade_of_pink);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, shade_of_pink);
		_step++;
	} else {
		_step = kMaxInputValueLastStages;
		_stage++;
	}
}

void SadCry::stage4()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage5()
{
	increaseBrightness();
}

void SadCry::stage6()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage7()
{
	increaseBrightness();
}

void SadCry::stage8()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage9()
{
	increaseBrightness();
}

void SadCry::stage10()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage11()
{
	increaseBrightness();
}

void SadCry::stage12()
{
	static constexpr auto kMaxInputValueLastStages = uint8_t {30};
	if (auto pos = mapStep(_step, 2 * kMaxInputValueLastStages); pos != 1.F) {
		_step++;
	} else {
		_step = kMaxInputValueLastStages;
		_stage++;
	}
}

void SadCry::stage13()
{
	decreaseBrightness(0.F);
}

void SadCry::increaseBrightness()
{
	static constexpr auto kMaxInputValueLastStages = uint8_t {30};
	static constexpr auto kNumberOfLedsBelt		   = uint8_t {20};

	static constexpr auto pink = RGB {255, 98, 98};

	if (auto pos = mapStep(_step, kMaxInputValueLastStages); pos != 1.F) {
		RGB shade_of_blue = ColorKit::colorGradient(RGB::black, RGB::pure_blue, pos);
		RGB shade_of_pink = ColorKit::colorGradient(RGB::black, pink, pos);
		_belt.setColor(shade_of_blue);
		_belt.setColorAtIndex(0, shade_of_pink);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, shade_of_pink);
		_step++;
	} else {
		_stage++;
	}
}

void SadCry::decreaseBrightness(float threshold)
{
	static constexpr auto kMaxInputValueLastStages = uint8_t {30};
	static constexpr auto kNumberOfLedsBelt		   = uint8_t {20};

	static constexpr auto pink = RGB {255, 98, 98};

	if (auto pos = mapStep(_step, kMaxInputValueLastStages); pos > threshold) {
		RGB shade_of_blue = ColorKit::colorGradient(RGB::black, RGB::pure_blue, pos);
		RGB shade_of_pink = ColorKit::colorGradient(RGB::black, pink, pos);
		_belt.setColor(shade_of_blue);
		_belt.setColorAtIndex(0, shade_of_pink);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, shade_of_pink);
		_step--;
	} else {
		_stage++;
	}
}

void SadCry::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
