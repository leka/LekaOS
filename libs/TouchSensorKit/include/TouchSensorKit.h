// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreTouchSensor.h"
#include "LogKit.h"
#include "internal/TouchSensorSystem.h"

namespace leka {

enum class Position
{
	ear_left,
	ear_right,
	belt_front_left,
	belt_front_right,
	belt_back_left,
	belt_back_right,
};

class TouchSensorKit
{
  public:
	explicit TouchSensorKit() = default;
	void setup();

	void updateState();
	void printState();

	void resetByPowerMode();

	void adjustSensitivity(uint16_t value, bool saved = false);

  private:
	CoreTouchSensor _sensor_ear_left {ear_left_in, _ear_left_pm, dac_touch_left, Channel::A};
	CoreTouchSensor _sensor_ear_right {ear_right_in, _ear_right_pm, dac_touch_right, ear_right_ch};
	CoreTouchSensor _sensor_belt_left_back {belt_left_back_in, _belt_left_back_pm, dac_touch_left, belt_left_back_ch};
	CoreTouchSensor _sensor_belt_left_front {belt_left_front_in, _belt_left_front_pm, dac_touch_left,
											 belt_left_front_ch};
	CoreTouchSensor _sensor_belt_right_back {belt_right_back_in, _belt_right_back_pm, dac_touch_right,
											 belt_right_back_ch};
	CoreTouchSensor _sensor_belt_right_front {belt_right_front_in, _belt_right_front_pm, dac_touch_right,
											  belt_right_front_ch};

	uint16_t default_max_sensitivity_value {0x0FFF};
	uint16_t default_min_sensitivity_value {0x0000};
};
}	// namespace leka
