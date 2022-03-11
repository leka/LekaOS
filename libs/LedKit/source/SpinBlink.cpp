// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "SpinBlink.h"

#include "MathUtils.h"

namespace leka::led::animation {

void SpinBlink::start()
{
	turnLedBlack();
}

void SpinBlink::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void SpinBlink::run()
{
	static constexpr auto kLastStage = uint8_t {41};
	auto is_belt_cyan				 = [this] { return (_stage % 2) == 0; };
	if (_stage > kLastStage) {
		return;
	}
	if (is_belt_cyan()) {
		stagesBeltCyan();
	} else {
		stagesBeltMagenta();
	}
	_ears.show();
	_belt.show();
}

auto SpinBlink::mapStep(uint8_t step) const -> float
{
	static constexpr auto kInputMaxValue = uint8_t {10};
	return utils::math::map(step, uint8_t {0}, kInputMaxValue, 0.F, 1.F);
}

void SpinBlink::stagesBeltCyan()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_ears.setColor(RGB::magenta);
		_belt.setColor(RGB::cyan);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void SpinBlink::stagesBeltMagenta()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_ears.setColor(RGB::cyan);
		_belt.setColor(RGB::magenta);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void SpinBlink::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
