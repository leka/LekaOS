// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "AnimationBouncingSquare.h"

#include "corevideo_config.h"

namespace leka {

AnimationBouncingSquare::AnimationBouncingSquare(LKCoreGraphicsBase &coregraphics) : _coregraphics(coregraphics) {}

void AnimationBouncingSquare::stop(void)
{
	_is_running = false;

	rtos::ThisThread::sleep_for(_refresh_rate * 10);

	_coregraphics.clearScreen(CGColor::black);
}

bool AnimationBouncingSquare::squareIsOutOfBoundVertical() const
{
	bool is_top_out_of_bound	= _square.origin.y + _vertical_shift <= 0;
	bool is_bottom_out_of_bound = _square.origin.y + _square.height + _vertical_shift > lcd::dimension.height - 1;

	return (is_top_out_of_bound || is_bottom_out_of_bound);
}

bool AnimationBouncingSquare::squareIsOutOfBoundHorizontal() const
{
	bool is_left_out_of_bound  = _square.origin.x + _horizontal_shift <= 0;
	bool is_right_out_of_bound = _square.origin.x + _square.width + _horizontal_shift > lcd::dimension.width - 1;

	return (is_left_out_of_bound || is_right_out_of_bound);
}

void AnimationBouncingSquare::updateDirection()
{
	if (squareIsOutOfBoundHorizontal()) {
		_horizontal_shift = -_horizontal_shift;
	}

	if (squareIsOutOfBoundVertical()) {
		_vertical_shift = -_vertical_shift;
	}
}

void AnimationBouncingSquare::updatePosition()
{
	_square.origin.x += _horizontal_shift;
	_square.origin.y += _vertical_shift;
}

void AnimationBouncingSquare::updateColor()
{
	if (_color.red == 0x00) {
		_color.green++;
		_color.blue--;
	}
	if (_color.green == 0x00) {
		_color.blue++;
		_color.red--;
	}
	if (_color.blue == 0x00) {
		_color.red++;
		_color.green--;
	}
}

void AnimationBouncingSquare::start(void)
{
	_is_running = true;

	_coregraphics.clearScreen(CGColor::yellow);
}

void AnimationBouncingSquare::run(void)
{
	while (_is_running) {
		// Change direction at screen limits
		updateDirection();

		// Update square position
		updatePosition();

		// Update square color
		updateColor();

		// Draw square
		_coregraphics.drawRectangle(_square, _color);

		// Wait some time
		rtos::ThisThread::sleep_for(_refresh_rate);
	}
}

}	// namespace leka
