// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreTouchSensor.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreTouchSensor::init()
{
	_detect_pin.mode(PinMode::PullUp);
	setPowerMode(PowerMode::normal);
	_sensitivity_pin.dac.init();
}

auto CoreTouchSensor::read() -> bool
{
	_state = (1 == _detect_pin.read());
	return _state;
}

void CoreTouchSensor::reset()
{
	setPowerMode(PowerMode::low);
	rtos::ThisThread::sleep_for(10ms);
	setPowerMode(PowerMode::normal);
	rtos::ThisThread::sleep_for(10ms);
}

void CoreTouchSensor::setSensitivity(uint16_t value, bool saved)
{
	_sensitivity_pin.dac.write(_sensitivity_pin.channel, value, saved);
}

void CoreTouchSensor::setPowerMode(PowerMode power_mode)
{
	auto pm = static_cast<uint8_t>(power_mode);
	_power_mode_pin.write(pm);
}
