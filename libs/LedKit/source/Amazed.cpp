// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Amazed.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Amazed::start()
{
	turnLedBlack();
}

void Amazed::stop()
{
	turnLedBlack();
}

void Amazed::run()
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

auto Amazed::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {30};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Amazed::stage0()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Amazed::stage1()
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

void Amazed::stage2()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		_belt.setColor(RGB::white);
		_step++;
	} else {
		_stage++;
	}
}

void Amazed::stage3()
{
	if (auto pos = mapStep(_step); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Amazed::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
