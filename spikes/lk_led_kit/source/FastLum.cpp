// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "FastLum.h"

namespace leka::led::animation {

void FastLum::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto FastLum::isRunning() -> bool
{
	return _running;
}

void FastLum::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;

	_running = true;
}

void FastLum::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void FastLum::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			animation1();
			break;
		case 1:
			animation2();
			break;
		case 2:
			animation3Part1();
			break;
		case 3:
			animation3Part2();
			break;

		default:
			_running = false;
			break;
	}
	_ears->show();
	_belt->show();
}
void FastLum::animation1()
{
	constexpr auto kMaxInputValue = 60;

	if (_step == 0) {
		_belt->setColorAtIndex(0, RGB::pure_red);
		_belt->setColorAtIndex(1, RGB::pure_green);
		_belt->setColorAtIndex(2, RGB::pure_blue);
		++_step;
	} else if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastLum::animation2()
{
	constexpr auto kMaxInputValue = 60;
	if (_step == kMaxInputValue) {
		_step = 0;
		turnLedBlack();
		++_stage;

	} else if (_step % 10 == 0) {
		_belt->setColorAtIndex(0, RGB::pure_red);
		++_step;

	} else if (_step % 10 == 5) {
		_belt->setColorAtIndex(0, RGB::black);
		++_step;
	} else {
		++_step;
	}
}

void FastLum::pureRedFor80BlackFor40Front()
{
	constexpr auto kNumberOfLedsBelt = 20;
	static uint8_t cnt				 = 0;

	if (cnt == kNumberOfLedsBelt) {
		_step = 0;
		turnLedBlack();
		cnt = 0;
		++_stage;
	}

	if (_step % 3 == 0) {
		_step = ++_step;

	} else if (_step % 3 == 2) {
		_belt->setColorAtIndex(cnt, RGB::black);
		_belt->setColorAtIndex(cnt + 1, RGB::pure_red);
		++_step;
		++cnt;
	} else {
		++_step;
	}
}
void FastLum::pureRedFor80BlackFor40Back()
{
	static uint8_t cnt = 19;
	if (cnt == 0) {
		_step = 0;
		cnt	  = 19;
		turnLedBlack();
		_stage = 2;
	}

	if (_step % 3 == 0) {
		_step = _step++;

	} else if (_step % 3 == 2) {
		_belt->setColorAtIndex(cnt, RGB::pure_red);
		_belt->setColorAtIndex(cnt + 1, RGB::black);
		--cnt;
	} else {
		++_step;
	}
}
void FastLum::animation3Part2()
{
	pureRedFor80BlackFor40Back();
}
void FastLum::animation3Part1()
{
	pureRedFor80BlackFor40Front();
}
void FastLum::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
