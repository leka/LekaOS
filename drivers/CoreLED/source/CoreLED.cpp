// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

using namespace leka;

auto CoreLED::setBeltColor(LEDColor color) -> void
{
    this->belt_current_color = color;
}

auto CoreLED::getBeltColor() -> LEDColor
{
	if (not getState()){return LEDColor::black;}
	return this->belt_current_color;
}

auto CoreLED::setState(bool state) -> void
{
	this->belt_is_on = state;
}

auto CoreLED::getState() const-> bool
{
	return this->belt_is_on;
}

auto CoreLED::showColor() -> void
{
    // TO DO
}

auto CoreLED::turnOff() -> void
{
    setState(false);
}

auto CoreLED::turnOn() -> void
{
	setState(true);
}


