// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Bubbles.h"

namespace leka::led::animation {

static constexpr auto kNumberOfBubbles = uint8_t {20};

void Bubbles::start()
{
	turnLedBlack();
}

void Bubbles::stop()
{
	turnLedBlack();
}

void Bubbles::run()
{
	static constexpr auto kLastStage = 12;
	if (_stage == 1) {
		stage1();
	} else if (_stage <= kLastStage) {
		stage2();
	}
	_belt.show();
}

void Bubbles::stage1()
{
	static constexpr auto kMaxInputValueStage1 = uint8_t {60};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage1); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Bubbles::stage2()
{
	static constexpr auto kMaxInputValueStage2 = uint8_t {15};

	static constexpr std::array<uint8_t, kNumberOfBubbles> kBubblesIndex = {2,	17, 9, 5,  13, 19, 7, 1,  18, 8,
																			16, 11, 4, 19, 14, 0,  2, 17, 9,  5};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage2); pos != 1.F) {
		if (_bubble_number - 2 >= 0) {
			auto previous_bubble_index = kBubblesIndex.at(_bubble_number - 2);
			decreaseBrightnessAtIndex(previous_bubble_index, pos);
		}
		if (_bubble_number - 1 >= 0) {
			auto previous_bubble_index = kBubblesIndex.at(_bubble_number - 1);
			decreaseBrightnessAtIndex(previous_bubble_index, pos);
		}
		if (_bubble_number < kNumberOfBubbles) {
			auto current_bubble_index = kBubblesIndex.at(_bubble_number);
			increaseBrightnessAtIndex(current_bubble_index, pos);
		}
		if (_bubble_number + 1 < kNumberOfBubbles) {
			auto current_bubble_index = kBubblesIndex.at(_bubble_number + 1);
			increaseBrightnessAtIndex(current_bubble_index, pos);
		}
		++_step;
	} else {
		_step = 0;
		_bubble_number += 2;
		++_stage;
	}
}

void Bubbles::increaseBrightnessAtIndex(uint8_t index, float position)
{
	auto color = ColorKit::colorGradient(RGB::black, RGB::pure_blue, position);
	_belt.setColorAtIndex(index, color);
}

void Bubbles::decreaseBrightnessAtIndex(uint8_t index, float position)
{
	auto color = ColorKit::colorGradient(RGB::pure_blue, RGB::black, position);
	_belt.setColorAtIndex(index, color);
}

void Bubbles::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
