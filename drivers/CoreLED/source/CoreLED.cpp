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
	_spi.write(frame::start);

	for (auto i = 0; i < (_size * 3); i += 3) {
		auto data = std::to_array<uint8_t>({brightness, _color.red, _color.green, _color.blue});
		_spi.write(data);
	}

	_spi.write(frame::reset);

	// TODO (@hugo) - frame end must be generated depending on the size of the led strip
	_spi.write(frame::end);
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
