// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

using namespace leka;

auto TouchSensorKit::isTouched() -> bool
{
	auto value = _pin.read();

	return (value != 0);
}

auto TouchSensorKit::setPin(::io::expanded::DigitalIn<> &pin) -> bool
{
	auto _pin = pin;

	return true;
}
