// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "DigitalOut.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "LogKit.h"
#include "internal/TouchSensorSystem.h"
namespace leka {

class TouchSensorKit
{
  public:
	explicit TouchSensorKit() = default;
	void setup();

	void set_pull_mode(PinMode mode);

	void set_power_mode(int power_mode);
	void power_mode_reset();

	void updateState();
	void printState();

  private:
	CoreI2C corei2c {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
	mbed::DigitalOut expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	CoreIOExpanderMCP23017 expander {corei2c, expander_reset};

	leka::io::expanded::DigitalIn<> _ear_left_input {expander, touch::pin::ear_left_input};
	leka::io::expanded::DigitalIn<> _ear_right_input {expander, touch::pin::ear_right_input};
	leka::io::expanded::DigitalIn<> _belt_left_back_input {expander, touch::pin::belt_left_back_input};
	leka::io::expanded::DigitalIn<> _belt_left_front_input {expander, touch::pin::belt_left_front_input};
	leka::io::expanded::DigitalIn<> _belt_right_back_input {expander, touch::pin::belt_right_back_input};
	leka::io::expanded::DigitalIn<> _belt_right_front_input {expander, touch::pin::belt_right_front_input};

	leka::io::expanded::DigitalOut<> _ear_left_pm {expander, touch::pin::ear_left_power_mode};
	leka::io::expanded::DigitalOut<> _ear_right_pm {expander, touch::pin::ear_right_power_mode};
	leka::io::expanded::DigitalOut<> _belt_left_back_pm {expander, touch::pin::belt_left_back_power_mode};
	leka::io::expanded::DigitalOut<> _belt_left_front_pm {expander, touch::pin::belt_left_front_power_mode};
	leka::io::expanded::DigitalOut<> _belt_right_back_pm {expander, touch::pin::belt_right_back_power_mode};
	leka::io::expanded::DigitalOut<> _belt_right_front_pm {expander, touch::pin::belt_right_front_power_mode};

	bool shadow_ear_left_touched;
	bool shadow_ear_right_touched;
	bool shadow_belt_left_back_touched;
	bool shadow_belt_left_front_touched;
	bool shadow_belt_right_back_touched;
	bool shadow_belt_right_front_touched;
};
}	// namespace leka
