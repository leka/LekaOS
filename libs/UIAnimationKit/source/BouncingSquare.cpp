// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "BouncingSquare.h"

namespace leka::animation {

BouncingSquare::BouncingSquare(interface::Graphics &coregraphics) : _coregraphics(coregraphics) {}

void BouncingSquare::start()
{
	_coregraphics.clearScreen(CGColor::yellow);
}

void BouncingSquare::stop()
{
	_coregraphics.clearScreen(CGColor::black);
}

void BouncingSquare::run()
{
	updateSquareDirection();
	updateSquarePosition();
	updateSquareColor();
	drawSquare();
}

void BouncingSquare::updateSquareDirection()
{
	if (squareWillBeOutOfBoundHorizontal()) {
		_shift.horizontal = -_shift.horizontal;
	}

	if (squareWillBeOutOfBoundVertical()) {
		_shift.vertical = -_shift.vertical;
	}
}

auto BouncingSquare::squareWillBeOutOfBoundHorizontal() const -> bool
{
	if (_square.origin.x + _shift.horizontal <= 0) {
		return true;
	}
	if (_square.origin.x + _square.width + _shift.horizontal > static_cast<uint32_t>(lcd::dimension::width - 1)) {
		return true;
	}
	return false;
}

auto BouncingSquare::squareWillBeOutOfBoundVertical() const -> bool
{
	if (_square.origin.y + _shift.vertical <= 0) {
		return true;
	}
	if (_square.origin.y + _square.height + _shift.vertical > static_cast<uint32_t>(lcd::dimension::height - 1)) {
		return true;
	}
	return false;
}

void BouncingSquare::updateSquarePosition()
{
	_square.origin.x += _shift.horizontal;
	_square.origin.y += _shift.vertical;
}

void BouncingSquare::updateSquareColor()
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

void BouncingSquare::drawSquare()
{
	_coregraphics.drawRectangle(_square, _color);
}
}	// namespace leka::animation

// LCOV_EXCL_STOP
