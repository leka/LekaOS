// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// MARK: GCOVR_EXCL_START

#include "BouncingSquare.h"

#include "corevideo_config.h"

namespace leka::animation {

BouncingSquare::BouncingSquare(LKCoreGraphicsBase &coregraphics) : _coregraphics(coregraphics) {}

void BouncingSquare::stop(void)
{
	_coregraphics.clearScreen(CGColor::black);
}

bool BouncingSquare::squareWillBeOutOfBoundVertical() const
{
	bool top_out_of_bound	 = _square.origin.y + _vertical_shift <= 0;
	bool bottom_out_of_bound = _square.origin.y + _square.height + _vertical_shift > lcd::dimension.height - 1;

	return (top_out_of_bound || bottom_out_of_bound);
}

bool BouncingSquare::squareWillBeOutOfBoundHorizontal() const
{
	bool left_out_of_bound	= _square.origin.x + _horizontal_shift <= 0;
	bool right_out_of_bound = _square.origin.x + _square.width + _horizontal_shift > lcd::dimension.width - 1;

	return (left_out_of_bound || right_out_of_bound);
}

void BouncingSquare::updateDirection()
{
	if (squareWillBeOutOfBoundHorizontal()) {
		_horizontal_shift = -_horizontal_shift;
	}

	if (squareWillBeOutOfBoundVertical()) {
		_vertical_shift = -_vertical_shift;
	}
}

void BouncingSquare::updatePosition()
{
	_square.origin.x += _horizontal_shift;
	_square.origin.y += _vertical_shift;
}

void BouncingSquare::updateColor()
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

void BouncingSquare::start(void)
{
	_coregraphics.clearScreen(CGColor::yellow);
}

void BouncingSquare::run(void)
{
	// Change direction at screen limits
	updateDirection();

	// Update square position
	updatePosition();

	// Update square color
	updateColor();

	// Draw square
	_coregraphics.drawRectangle(_square, _color);
}

}	// namespace leka::animation

// MARK: GCOVR_EXCL_STOP
