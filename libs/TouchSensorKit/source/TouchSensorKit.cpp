// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

using namespace leka;

void TouchSensorKit::updateState()
{
	auto value						= _pin.read();
	shadow_ear_left_touched			= (0 == (value & touch::pin::ear_left));
	shadow_ear_right_touched		= (0 == (value & touch::pin::ear_right));
	shadow_belt_left_back_touched	= (0 == (value & touch::pin::belt_left_back));
	shadow_belt_left_front_touched	= (0 == (value & touch::pin::belt_left_front));
	shadow_belt_right_back_touched	= (0 == (value & touch::pin::belt_right_back));
	shadow_belt_right_front_touched = (0 == (value & touch::pin::belt_right_front));
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
