// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

using namespace leka;

void CoreLED::setColor(RGB color)
{
	_previous_color = _color;
	_color			= color;
}

auto CoreLED::isOn() const -> bool
{
	return _is_on;
}

void CoreLED::showColor()
{
	_spi.write(start_frame);

	for (auto i = 0; i < (_n_LEDs * 3); i += 3) {
		auto data = std::to_array<uint8_t>({brightness, _color.red, _color.green, _color.blue});
		_spi.write(data);
	}

	_spi.write(reset_frame);

	// At least N_LEDS/2 bits of 0 for the end frame ()
	_spi.write(end_frame);
}

void CoreLED::turnOff()
{
	_is_on = false;
	setColor(RGB::black);
	showColor();
}

void CoreLED::turnOn()
{
	setColor(_previous_color);
	_is_on = true;
	showColor();
}
