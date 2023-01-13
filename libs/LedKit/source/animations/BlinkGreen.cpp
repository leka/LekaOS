// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "BlinkGreen.h"

namespace leka::led::animation {

static constexpr auto green1 = RGB {0, 170, 0};
static constexpr auto green2 = RGB {0, 255, 80};

void BlinkGreen::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto BlinkGreen::isRunning() -> bool
{
	return _running;
}

void BlinkGreen::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void BlinkGreen::stop()
{
	_running = false;

	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
}

void BlinkGreen::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	static constexpr auto kLastStage = uint8_t {20};
	auto is_green2_stage			 = [this] { return (_stage % 4) == 0; };
	auto is_green1_stage			 = [this] { return (_stage % 2) == 0; };
	if (_stage > kLastStage) {
		turnLedBlack();
		_running = false;
		return;
	}
	if (is_green2_stage()) {
		stagesGreen2();
	} else if (is_green1_stage()) {
		stagesGreen1();
	} else {
		stagesBlack();
	}
	_belt->show();
	_ears->show();
}

void BlinkGreen::stagesBlack()
{
	static constexpr auto kMaxInputValue = uint8_t {6};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColor(RGB::black);
		_ears->setColor(RGB::black);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkGreen::stagesGreen1()
{
	static constexpr auto kMaxInputValue = uint8_t {6};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColor(green1);
		_ears->setColor(green1);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkGreen::stagesGreen2()
{
	static constexpr auto kMaxInputValue = uint8_t {6};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColor(green2);
		_ears->setColor(green2);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void BlinkGreen::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
