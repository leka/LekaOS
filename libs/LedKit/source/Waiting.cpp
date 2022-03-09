// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Waiting.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Waiting::start()
{
	turnLedBlack();
}

void Waiting::stop()
{
	turnLedBlack();
}

void Waiting::run()
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
		default:
			break;
	}
	_belt.show();
}

auto Waiting::mapStep(uint8_t step, uint8_t max_input_value) const -> float
{
	return utils::math::map(step, uint8_t {0}, max_input_value, 0.F, 1.F);
}

void Waiting::stage1()
{
	static constexpr auto kMaxInputValue1 = uint8_t {70};
	static constexpr auto kMaxInputValue2 = uint8_t {50};
	if (auto pos = mapStep(_step, kMaxInputValue1); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_step = kMaxInputValue2;
		_stage++;
	}
}

void Waiting::stage2()
{
	static constexpr auto tresholdDown	  = 0.3F;
	static constexpr auto kMaxInputValue2 = uint8_t {50};
	if (auto pos = mapStep(_step, kMaxInputValue2); pos >= tresholdDown) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Waiting::stage3()
{
	static constexpr auto kMaxInputValue1 = uint8_t {70};
	static constexpr auto kMaxInputValue2 = uint8_t {50};
	if (auto pos = mapStep(_step, kMaxInputValue2); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_step = kMaxInputValue1;
		_stage++;
	}
}

void Waiting::stage4()
{
	static constexpr auto kMaxInputValue1 = uint8_t {70};
	if (auto pos = mapStep(_step, kMaxInputValue1); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Waiting::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
