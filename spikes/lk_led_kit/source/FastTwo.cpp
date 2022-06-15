// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "FastTwo.h"

namespace leka::led::animation {

void FastTwo::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto FastTwo::isRunning() -> bool
{
	return _running;
}

void FastTwo::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;

	_running = true;
}

void FastTwo::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void FastTwo::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			animation4();
			break;
		case 1:
			animation5();
			break;
		case 2:
			animation6();
			break;
		case 3:
			animation7();
			break;

		default:
			_running = false;
			break;
	}
	_ears->show();
	_belt->show();
}
void FastTwo::animation4()
{
	constexpr auto kMaxInputValue = 20;

	rangeGradient2Colors(RGB::magenta, RGB::yellow);

	if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastTwo::animation5()
{
	constexpr auto kMaxInputValue = 20;

	rangeGradient3Colors(RGB::pure_red, RGB::pure_green, RGB::pure_blue);

	if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}

void FastTwo::animation6()
{
	constexpr auto kMaxInputValue = 20;

	rangeGradient4Colors(RGB::pure_blue, RGB::pure_green, RGB::pure_red, RGB::yellow);

	if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}

void FastTwo::animation7()
{
	constexpr auto kMaxInputValue = 20;

	rangeGradient5Colors(RGB::pure_blue, RGB::pure_green, RGB::pure_red, RGB::yellow, RGB::magenta);

	if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}

void FastTwo::rangeGradient2Colors(const RGB &color1, const RGB &color2)
{
	for (uint8_t index(0); index < 19; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {19}, 0.F, 1.F);
		RGB color		  = ColorKit::colorGradient(color1, color2, index_color);
		_belt->setColorAtIndex(index, color);
	}
}
void FastTwo::rangeGradient3Colors(const RGB &color1, const RGB &color2, const RGB &color3)
{
	for (uint8_t index(0); index < 19; index++) {
		if (index < 10) {
			float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {10}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color1, color2, index_color);
			_belt->setColorAtIndex(index, color);
		} else {
			float index_color = leka::utils::math::map(index, uint8_t {10}, uint8_t {19}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color2, color3, index_color);
			_belt->setColorAtIndex(index, color);
		}
	}
}
void FastTwo::rangeGradient4Colors(const RGB &color1, const RGB &color2, const RGB &color3, const RGB &color4)
{
	for (uint8_t index(0); index < 19; index++) {
		if (index <= 6) {
			float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {6}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color1, color2, index_color);
			_belt->setColorAtIndex(index, color);
		} else if (index <= 12) {
			float index_color = leka::utils::math::map(index, uint8_t {6}, uint8_t {12}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color2, color3, index_color);
			_belt->setColorAtIndex(index, color);
		} else {
			float index_color = leka::utils::math::map(index, uint8_t {12}, uint8_t {19}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color3, color4, index_color);
			_belt->setColorAtIndex(index, color);
		}
	}
}
void FastTwo::rangeGradient5Colors(const RGB &color1, const RGB &color2, const RGB &color3, const RGB &color4,
								   const RGB &color5)
{
	for (uint8_t index(0); index < 19; index++) {
		if (index <= 5) {
			float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {5}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color1, color2, index_color);
			_belt->setColorAtIndex(index, color);
		} else if (index <= 10) {
			float index_color = leka::utils::math::map(index, uint8_t {5}, uint8_t {10}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color2, color3, index_color);
			_belt->setColorAtIndex(index, color);
		} else if (index <= 15) {
			float index_color = leka::utils::math::map(index, uint8_t {10}, uint8_t {15}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color3, color4, index_color);
			_belt->setColorAtIndex(index, color);
		} else {
			float index_color = leka::utils::math::map(index, uint8_t {15}, uint8_t {19}, 0.F, 1.F);
			RGB color		  = ColorKit::colorGradient(color4, color5, index_color);
			_belt->setColorAtIndex(index, color);
		}
	}
}

void FastTwo::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
