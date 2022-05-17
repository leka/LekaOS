// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono;

void TouchSensorKit::setup()
{
	set_pull_mode(PinMode::PullUp);
	set_power_mode(touch::power_mode::normal);
}

void TouchSensorKit::set_pull_mode(PinMode mode)
{
	_ear_left_input.mode(mode);
	_ear_right_input.mode(mode);
	_belt_left_back_input.mode(mode);
	_belt_left_front_input.mode(mode);
	_belt_right_back_input.mode(mode);
	_belt_right_front_input.mode(mode);
}

void TouchSensorKit::set_power_mode(int power_mode)
{
	_ear_left_pm.write(power_mode);
	_ear_right_pm.write(power_mode);
	_belt_left_back_pm.write(power_mode);
	_belt_left_front_pm.write(power_mode);
	_belt_right_back_pm.write(power_mode);
	_belt_right_front_pm.write(power_mode);
}

void TouchSensorKit::power_mode_reset()
{
	set_power_mode(touch::power_mode::low);
	rtos::ThisThread::sleep_for(5ms);
	set_power_mode(touch::power_mode::normal);
}

void TouchSensorKit::updateState()
{
	shadow_ear_left_touched			= (0 == _ear_left_input.read());
	shadow_ear_right_touched		= (0 == _ear_right_input.read());
	shadow_belt_left_back_touched	= (0 == _belt_left_back_input.read());
	shadow_belt_left_front_touched	= (0 == _belt_left_front_input.read());
	shadow_belt_right_back_touched	= (0 == _belt_right_back_input.read());
	shadow_belt_right_front_touched = (0 == _belt_right_front_input.read());
	power_mode_reset();
}

void TouchSensorKit::printState()
{
	log_info("Ear left touched: %s", shadow_ear_left_touched ? "true" : "false");
	log_info("Ear right touched: %s", shadow_ear_right_touched ? "true" : "false");
	log_info("Belt left front touched: %s", shadow_belt_left_front_touched ? "true" : "false");
	log_info("Belt left back touched: %s", shadow_belt_left_back_touched ? "true" : "false");
	log_info("Belt right front touched: %s", shadow_belt_right_front_touched ? "true" : "false");
	log_info("Belt right back touched: %s", shadow_belt_right_back_touched ? "true" : "false");
}
