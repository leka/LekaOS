// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// MARK: GCOVR_EXCL_START

#include "UnrealRainbow.h"

#include "corevideo_config.h"

namespace leka::animation {

bool isAtBoundaries(uint8_t value)
{
	return (value == 0x00 || value == 0xFF);
}

void UnrealRainbow::updateDirections()
{
	// Update color value direction
	_dir = -_dir;

	// Select channel updated
	if (_channel == &_color.red) {
		_channel = &_color.blue;
	} else if (_channel == &_color.green) {
		_channel = &_color.red;
	} else if (_channel == &_color.blue) {
		_channel = &_color.green;
	}
}

void UnrealRainbow::run(void)
{
	// Check color value of selected channel is at limit
	if (isAtBoundaries(*_channel)) {
		updateDirections();
	}

	// Update color value of selected channel
	*_channel += _dir;

	// Apply color
	_coregraphics.clearScreen(_color);
}

void UnrealRainbow::stop(void)
{
	_coregraphics.clearScreen(CGColor::black);
}

}	// namespace leka::animation

// MARK: GCOVR_EXCL_STOP
