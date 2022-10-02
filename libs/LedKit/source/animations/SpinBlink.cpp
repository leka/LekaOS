// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "SpinBlink.h"

namespace leka::led::animation {

void SpinBlink::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto SpinBlink::isRunning() -> bool
{
	return _running;
}

void SpinBlink::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void SpinBlink::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void SpinBlink::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	static constexpr auto kLastStage = uint8_t {10};
	auto is_belt_cyan				 = [this] { return (_stage % 2) == 0; };
	if (_stage > kLastStage) {
		turnLedBlack();
		_running = false;
		return;
	}
	if (is_belt_cyan()) {
		stagesBeltCyan();
	} else {
		stagesBeltMagenta();
	}
	_ears->show();
	_belt->show();
}

void SpinBlink::stagesBeltCyan()
{
	static constexpr auto kMaxInputValue = uint8_t {10};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_ears->setColor(RGB::magenta);
		_belt->setColor(RGB::cyan);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void SpinBlink::stagesBeltMagenta()
{
	static constexpr auto kMaxInputValue = uint8_t {10};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_ears->setColor(RGB::cyan);
		_belt->setColor(RGB::magenta);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void SpinBlink::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
