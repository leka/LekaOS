// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

using namespace leka;

void CoreLED::setColor(RGB color)
{
	_color = color;
}

void CoreLED::showColor()
{
	sendAndDisplay(_color);
	_is_color_shown = true;
}

void CoreLED::hideColor()
{
	if (isOn()) {
		sendAndDisplay(RGB::black);
		_is_color_shown = false;
	}
}

auto CoreLED::isOn() -> bool
{
	return _is_color_shown && _color != RGB::black;
}

void CoreLED::sendAndDisplay(RGB color)
{
	_spi.write(frame::start);

	for (auto i = 0; i < (_size * 3); i += 3) {
		auto data = std::to_array<uint8_t>({brightness, color.red, color.green, color.blue});
		_spi.write(data);
	}

	_spi.write(frame::reset);

	// TODO (@hugo) - frame end must be generated depending on the size of the led strip
	_spi.write(frame::end);
}
