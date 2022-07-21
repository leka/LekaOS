// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreTouchSensor.h"

using namespace leka;

auto CoreTouchSensor::read() -> int
{
	return _detect_pin.read();
}

void CoreTouchSensor::setPowerMode(PowerMode power_mode)
{
	auto pm = static_cast<uint8_t>(power_mode);
	_power_mode_pin.write(pm);
}

void CoreTouchSensor::setSensitivity(uint16_t value, bool saved)
{
	_sensitivity_pin.dac.write(_sensitivity_pin.channel, value, saved);
}
