// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreTouchSensor.h"

#include "rtos/ThisThread.h"

#include "MathUtils.h"

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
	rtos::ThisThread::sleep_for(1ms);
	setPowerMode(PowerMode::normal);
	rtos::ThisThread::sleep_for(1ms);
}

void CoreTouchSensor::setSensitivity(uint16_t value, bool saved)
{
	auto inverted_value =
		utils::math::map<uint16_t, uint16_t>(value, default_min_sensitivity_value, default_max_sensitivity_value,
											 default_max_sensitivity_value, default_min_sensitivity_value);
	_sensitivity_pin.dac.write(_sensitivity_pin.channel, inverted_value, saved);
}

void CoreTouchSensor::setPowerMode(PowerMode power_mode)
{
	auto pm = static_cast<uint8_t>(power_mode);
	_power_mode_pin.write(pm);
}
