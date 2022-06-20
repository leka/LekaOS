// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Sinus.h"

namespace leka::led::animation {

void Sinus::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Sinus::isRunning() -> bool
{
	return _running;
}

void Sinus::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Sinus::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Sinus::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			ColorFluctuation();
			break;
		case 1:
			ColorFluctuationOneLedSimpleSinusoid();
			break;
		case 2:
			ColorFluctuationOneLedComplexSinusoid();
			break;
		case 3:
			ColorFluctuationThreeLedSimpleSinusoid();
			break;
		default:
			_running = false;
			break;
	}
	_ears->show();
	_belt->show();
}

void Sinus::ColorFluctuation()
{
	static constexpr auto kMaxInputValue = uint8_t {100};
	if (_step < kMaxInputValue) {
		auto sin_pos = leka::utils::math::sinPeriod(30, _step, 0, 0.F, 1.F);
		RGB color	 = ColorKit::colorGradient(RGB::pure_blue, RGB::magenta, sin_pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Sinus::ColorFluctuationOneLedSimpleSinusoid()
{
	static constexpr auto kMaxInputValue = uint8_t {150};
	if (_step < kMaxInputValue) {
		auto sin_pos   = leka::utils::math::sinPeriod(50, _step, 0, 0.F, 1.F);
		auto sin_index = leka::utils::math::map(sin_pos, 0.F, 1.F, uint8_t {0}, uint8_t {19});
		RGB color	   = ColorKit::colorGradient(RGB::pure_green, RGB::pure_red, sin_pos);
		_belt->setColor(RGB::black);
		_belt->setColorAtIndex(sin_index, color);
		_ears->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Sinus::ColorFluctuationOneLedComplexSinusoid()
{
	static constexpr auto kMaxInputValue = uint8_t {255};
	if (_step < kMaxInputValue) {
		auto sin_pos1  = leka::utils::math::sinPeriod(50, _step, 0, 0.F, 1.F);
		auto sin_pos2  = leka::utils::math::sinPeriod(150, _step, 0, 0.F, 1.F);
		auto sin_pos   = (sin_pos1 + sin_pos2) / 2.F;
		auto sin_index = leka::utils::math::map(sin_pos, 0.F, 1.F, uint8_t {0}, uint8_t {19});
		RGB color	   = ColorKit::colorGradient(RGB::yellow, RGB::cyan, sin_pos);
		_belt->setColor(RGB::black);
		_belt->setColorAtIndex(sin_index, color);
		_ears->setColor(color);
		_step++;
	} else {
		_belt->setColor(RGB::black);
		_step = 0;
		_stage++;
	}
}

void Sinus::ColorFluctuationThreeLedSimpleSinusoid()
{
	static constexpr auto kMaxInputValue = uint8_t {255};
	if (_step < kMaxInputValue) {
		auto sin_pos1	= leka::utils::math::sinPeriod(50, _step, 0, 0.F, 1.F);
		auto sin_pos2	= leka::utils::math::sinPeriod(50, _step, 100, 0.F, 1.F);
		auto sin_pos3	= leka::utils::math::sinPeriod(50, _step, 200, 0.F, 1.F);
		auto sin_index1 = leka::utils::math::map(sin_pos1, 0.F, 1.F, uint8_t {0}, uint8_t {19});
		auto sin_index2 = leka::utils::math::map(sin_pos2, 0.F, 1.F, uint8_t {0}, uint8_t {19});
		auto sin_index3 = leka::utils::math::map(sin_pos3, 0.F, 1.F, uint8_t {0}, uint8_t {19});
		_belt->setColor(RGB::black);
		_belt->setColorAtIndex(sin_index1, RGB::cyan);
		_belt->setColorAtIndex(sin_index2, RGB::yellow);
		_belt->setColorAtIndex(sin_index3, RGB::magenta);
		_step++;
	} else {
		_belt->setColor(RGB::black);
		_step = 0;
		_stage++;
	}
}

void Sinus::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
