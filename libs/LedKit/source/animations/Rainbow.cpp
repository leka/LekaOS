// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Rainbow.h"

namespace leka::led::animation {

void Rainbow::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

void Rainbow::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step				= 0;
	_stage				= 0;
	_rainbow_tail_index = 0;
}

void Rainbow::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
}

void Rainbow::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	if (auto kLastStage = uint8_t {150}; _stage <= kLastStage) {
		stagesRainbow();
	} else {
		turnLedBlack();
	}
	_belt->show();
}

void Rainbow::stagesRainbow()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (_rainbow_tail_index == kNumberOfLedsBelt) {
		_rainbow_tail_index = 0;
	}
	for (auto i = _rainbow_tail_index; i < _rainbow_tail_index + kNumberOfLedsBelt; i++) {
		if (i < kNumberOfLedsBelt) {
			RGB color = getRainbowColor(i - _rainbow_tail_index);
			_belt->setColorAtIndex(i, color);
		} else {
			RGB color = getRainbowColor(i - _rainbow_tail_index);
			_belt->setColorAtIndex(i - kNumberOfLedsBelt, color);
		}
	}
	++_rainbow_tail_index;
	++_stage;
}

auto Rainbow::getRainbowColor(uint8_t index) const -> RGB
{
	static constexpr auto kRainbowColorNumber						   = 7;
	static constexpr std::array<RGB, kRainbowColorNumber> rainbowColor = {
		RGB::pure_red,	RGB {180, 50, 0},	 RGB {100, 100, 0}, RGB::pure_green,
		RGB::pure_blue, RGB {0x4b, 0, 0x82}, RGB::magenta};

	auto part_of_rainbow = index / 3;
	auto position		 = static_cast<float>(index % 3) / 3;
	if (part_of_rainbow < kRainbowColorNumber - 1) {
		RGB color =
			ColorKit::colorGradient(rainbowColor.at(part_of_rainbow), rainbowColor.at(part_of_rainbow + 1), position);
		return color;
	}
	return rainbowColor.at(kRainbowColorNumber - 1);
}

void Rainbow::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
