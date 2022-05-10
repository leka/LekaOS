// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Rainbow.h"

namespace leka::led::animation {

static constexpr auto kNumberOfLedsBelt = uint8_t {20};

void Rainbow::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Rainbow::isRunning() -> bool
{
	return _running;
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
	_running			= true;
}

void Rainbow::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
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
		_running = false;
	}
	_belt->show();
}

void Rainbow::stagesRainbow()
{
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
	static constexpr auto colors = std::to_array<RGB>({
		RGB::pure_red,
		RGB::pure_green,
		RGB::pure_blue,
		RGB::magenta,
		RGB::pure_red,
	});

	static constexpr auto kGradientLength = kNumberOfLedsBelt / colors.size();

	auto colors_index = index / kGradientLength;
	auto gradient_pos = static_cast<float>(index % kGradientLength) / kGradientLength;

	if (colors_index < colors.size() - 1) {
		RGB color = ColorKit::colorGradient(colors.at(colors_index), colors.at(colors_index + 1), gradient_pos);
		return color;
	}

	return colors.at(colors.size() - 1);
}

void Rainbow::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
