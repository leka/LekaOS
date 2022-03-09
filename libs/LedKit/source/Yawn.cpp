// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Yawn.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Yawn::start()
{
	turnLedBlack();
}

void Yawn::stop()
{
	turnLedBlack();
}

void Yawn::run()
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
		default:
			break;
	}
	_belt.show();
}

auto Yawn::mapStep(uint8_t step, uint8_t max_input_value) const -> float
{
	return utils::math::map(step, uint8_t {0}, max_input_value, 0.F, 1.F);
}

void Yawn::stage0()
{
	static constexpr auto kInputMaxValue1 = uint8_t {20};
	if (auto pos = mapStep(_step, kInputMaxValue1); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Yawn::stage1()
{
	static constexpr auto kInputMaxValue1 = uint8_t {20};
	if (auto pos = mapStep(_step, kInputMaxValue1); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Yawn::stage2()
{
	static constexpr auto kInputMaxValue1 = uint8_t {20};
	static constexpr auto kInputMaxValue2 = uint8_t {5};
	if (auto pos = mapStep(_step, kInputMaxValue2); pos != 1.F) {
		++_step;
	} else {
		_step = kInputMaxValue1;
		++_stage;
	}
}

void Yawn::stage3()
{
	static constexpr auto kInputMaxValue1 = uint8_t {20};
	if (auto pos = mapStep(_step, kInputMaxValue1); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		--_step;
	} else {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		++_stage;
	}
}

void Yawn::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
