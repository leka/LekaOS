// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "BlinkGreen.h"

#include "MathUtils.h"

namespace leka::led::animation {

static constexpr auto green1 = RGB {0, 170, 0};
static constexpr auto green2 = RGB {0, 255, 80};

void BlinkGreen::start()
{
	turnLedBlack();
}

void BlinkGreen::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void BlinkGreen::run()
{
	static constexpr auto kLastStage = uint8_t {41};
	auto is_green2_stage			 = [this] { return (_stage % 4) == 0; };
	auto is_green1_stage			 = [this] { return (_stage % 2) == 0; };
	if (_stage > kLastStage) {
		return;
	}
	if (is_green2_stage()) {
		stagesGreen2();
	} else if (is_green1_stage()) {
		stagesGreen1();
	} else {
		stagesBlack();
	}
	_belt.show();
	_ears.show();
}

auto BlinkGreen::mapStep(uint8_t step) const -> float
{
	static constexpr auto kInputMaxValue = uint8_t {6};
	return utils::math::map(step, uint8_t {0}, kInputMaxValue, 0.F, 1.F);
}

void BlinkGreen::stagesBlack()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColor(RGB::black);
		_ears.setColor(RGB::black);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkGreen::stagesGreen1()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColor(green1);
		_ears.setColor(green1);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkGreen::stagesGreen2()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColor(green2);
		_ears.setColor(green2);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkGreen::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
