// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report
#include <cmath>
#include <numbers>

#include "LogKit.h"
#include "MathUtils.h"
#include "Waiting.h"

namespace leka::animation {

static constexpr auto in_max			= 10;
static constexpr auto kNumberOfLedsBelt = 20;

static constexpr std::array<RGB, kNumberOfLedsBelt> rainbow_array = {
	RGB::pure_red,	   RGB {255, 38, 0},   RGB {255, 92, 0},   RGB {255, 165, 0},	   RGB {255, 193, 0},
	RGB {255, 222, 0}, RGB::yellow,		   RGB {137, 208, 0},  RGB {53, 166, 0},	   RGB {0, 128, 0},
	RGB {0, 85, 67},   RGB {0, 43, 150},   RGB::pure_blue,	   RGB {17, 3, 208},	   RGB {42, 3, 166},
	RGB {75, 0, 130},  RGB {117, 24, 165}, RGB {171, 66, 201}, RGB {0xEE, 0x82, 0xEE}, RGB {0xEE, 0x82, 0xEE},
};

Waiting::Waiting(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {}

void Waiting::start()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

void Waiting::stop()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

void Waiting::run()
{
	updateRainbow();
	_belt.show();
}

auto Waiting::mapStep(uint8_t in_max) const -> float
{
	return utils::math::map(_step, static_cast<uint8_t>(0), in_max, 0.F, 1.F);
}

void Waiting::updateRainbow()
{
	float step_mapped = mapStep(in_max);
	if (step_mapped <= 1.F) {
		setColorCurrentMinusOne(_stage - 1, step_mapped);
		setColorCurrent(_stage, step_mapped);
		setColorCurrentPlusOne(_stage + 1, step_mapped);
		setColorCurrentPlusTwo(_stage + 2, step_mapped);
		setColorCurrentPlusThree(_stage + 3, step_mapped);
		++_step;
	} else {
		_step = 0;
		_stage += 2;
	}
}

void Waiting::setColorCurrentMinusOne(uint8_t index, float step_mapped)
{
	if (index >= 0 && index < kNumberOfLedsBelt) {
		if (step_mapped <= 0.5F) {
			RGB color = ColorKit::colorGradient(RGB::black, rainbow_array.at(index), 0.5F - step_mapped);
			_belt.setColorAtIndex(index, color);
		}
	}
}

void Waiting::setColorCurrent(uint8_t index, float step_mapped)
{
	if (index >= 0 && index < kNumberOfLedsBelt) {
		RGB color = ColorKit::colorGradient(RGB::black, rainbow_array.at(index), 1.F - step_mapped);
		_belt.setColorAtIndex(index, color);
	}
}

void Waiting::setColorCurrentPlusOne(uint8_t index, float step_mapped)
{
	if (index >= 0 && index < kNumberOfLedsBelt) {
		if (step_mapped <= 0.5F) {
			RGB color = ColorKit::colorGradient(RGB::black, rainbow_array.at(index), step_mapped + 0.5F);
			_belt.setColorAtIndex(index, color);
		} else {
			RGB color = ColorKit::colorGradient(RGB::black, rainbow_array.at(index), 1.5F - step_mapped);
			_belt.setColorAtIndex(index, color);
		}
	}
}

void Waiting::setColorCurrentPlusTwo(uint8_t index, float step_mapped)
{
	if (index >= 0 && index < kNumberOfLedsBelt) {
		RGB color = ColorKit::colorGradient(RGB::black, rainbow_array.at(index), step_mapped);
		_belt.setColorAtIndex(index, color);
	}
}

void Waiting::setColorCurrentPlusThree(uint8_t index, float step_mapped)
{
	if (index >= 0 && index < kNumberOfLedsBelt) {
		if (step_mapped >= 0.5F) {
			RGB color = ColorKit::colorGradient(RGB::black, rainbow_array.at(index), step_mapped - 0.5F);
			_belt.setColorAtIndex(index, color);
		}
	}
}

}	// namespace leka::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
