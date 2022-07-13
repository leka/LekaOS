// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::setup()
{
	setPowerMode(touch::power_mode::normal);
}

void TouchSensorKit::updateState()
{
	_state.ear_left_touched			= (0 == _sensor_ear_left.read());
	_state.ear_right_touched		= (0 == _sensor_ear_right.read());
	_state.belt_left_back_touched	= (0 == _sensor_belt_left_back.read());
	_state.belt_left_front_touched	= (0 == _sensor_belt_left_front.read());
	_state.belt_right_back_touched	= (0 == _sensor_belt_right_back.read());
	_state.belt_right_front_touched = (0 == _sensor_belt_right_front.read());
}

void TouchSensorKit::printState()
{
	log_info("Ear left touched: %s", _state.ear_left_touched ? "true" : "false");
	log_info("Ear right touched: %s", _state.ear_right_touched ? "true" : "false");
	log_info("Belt left front touched: %s", _state.belt_left_front_touched ? "true" : "false");
	log_info("Belt left back touched: %s", _state.belt_left_back_touched ? "true" : "false");
	log_info("Belt right front touched: %s", _state.belt_right_front_touched ? "true" : "false");
	log_info("Belt right back touched: %s", _state.belt_right_back_touched ? "true" : "false");
}

void TouchSensorKit::resetByPowerMode()
{
	if (_state.ear_left_touched | _state.ear_right_touched | _state.belt_left_front_touched |
		_state.belt_left_back_touched | _state.belt_right_front_touched | _state.belt_right_back_touched) {
		setPowerMode(touch::power_mode::low);
		rtos::ThisThread::sleep_for(5ms);
		setPowerMode(touch::power_mode::normal);
		rtos::ThisThread::sleep_for(5ms);
	}
}

void TouchSensorKit::setPowerMode(int power_mode)
{
	_sensor_ear_left.setPowerMode(power_mode);
	_sensor_ear_right.setPowerMode(power_mode);
	_sensor_belt_left_back.setPowerMode(power_mode);
	_sensor_belt_left_front.setPowerMode(power_mode);
	_sensor_belt_right_back.setPowerMode(power_mode);
	_sensor_belt_right_front.setPowerMode(power_mode);
}

void TouchSensorKit::adjustSensitivity(uint16_t value, bool saved)
{
	_sensor_ear_left.adjustSensitivity(value, saved);
	_sensor_ear_right.adjustSensitivity(value, saved);
	_sensor_belt_left_back.adjustSensitivity(value, saved);
	_sensor_belt_left_front.adjustSensitivity(value, saved);
	_sensor_belt_right_back.adjustSensitivity(value, saved);
	_sensor_belt_right_front.adjustSensitivity(value, saved);
	rtos::ThisThread::sleep_for(1ms);
}
