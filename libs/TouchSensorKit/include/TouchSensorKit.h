// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../../spikes/lk_game_touch_speed/Position.h"
#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "CoreQDAC.h"
#include "CoreTouchSensor.h"
#include "DigitalOut.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "LogKit.h"
#include "external/TouchSensorSystem.h"

namespace leka {

class TouchSensorKit
{
  public:
	explicit TouchSensorKit() = default;
	void setup();

	void updateState();
	void printState();

	void resetByPowerMode();

	void adjustSensitivity(uint16_t value, bool saved = false);

	auto component_touched(Position component) const -> bool;

  private:
	void setPowerMode(int power_mode);

	CoreI2C corei2c {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
	mbed::DigitalOut io_expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	CoreIOExpanderMCP23017 io_expander {corei2c, io_expander_reset};

	leka::io::expanded::DigitalIn<> _ear_left_input {io_expander, touch::pin::ear_left_input};
	leka::io::expanded::DigitalIn<> _ear_right_input {io_expander, touch::pin::ear_right_input};
	leka::io::expanded::DigitalIn<> _belt_left_back_input {io_expander, touch::pin::belt_left_back_input};
	leka::io::expanded::DigitalIn<> _belt_left_front_input {io_expander, touch::pin::belt_left_front_input};
	leka::io::expanded::DigitalIn<> _belt_right_back_input {io_expander, touch::pin::belt_right_back_input};
	leka::io::expanded::DigitalIn<> _belt_right_front_input {io_expander, touch::pin::belt_right_front_input};

	leka::io::expanded::DigitalOut<> _ear_left_pm {io_expander, touch::pin::ear_left_power_mode};
	leka::io::expanded::DigitalOut<> _ear_right_pm {io_expander, touch::pin::ear_right_power_mode};
	leka::io::expanded::DigitalOut<> _belt_left_back_pm {io_expander, touch::pin::belt_left_back_power_mode};
	leka::io::expanded::DigitalOut<> _belt_left_front_pm {io_expander, touch::pin::belt_left_front_power_mode};
	leka::io::expanded::DigitalOut<> _belt_right_back_pm {io_expander, touch::pin::belt_right_back_power_mode};
	leka::io::expanded::DigitalOut<> _belt_right_front_pm {io_expander, touch::pin::belt_right_front_power_mode};

	CoreQDACMCP4728 dac_touch_left {corei2c, 0xC0};
	CoreQDACMCP4728 dac_touch_right {corei2c, 0xC2};

	CoreTouchSensor _sensor_ear_left {_ear_left_input, _ear_left_pm, dac_touch_left, touch::pin::ear_left_ch};
	CoreTouchSensor _sensor_ear_right {_ear_right_input, _ear_right_pm, dac_touch_right, touch::pin::ear_right_ch};
	CoreTouchSensor _sensor_belt_left_back {_belt_left_back_input, _belt_left_back_pm, dac_touch_left,
											touch::pin::belt_left_back_ch};
	CoreTouchSensor _sensor_belt_left_front {_belt_left_front_input, _belt_left_front_pm, dac_touch_left,
											 touch::pin::belt_left_front_ch};
	CoreTouchSensor _sensor_belt_right_back {_belt_right_back_input, _belt_right_back_pm, dac_touch_right,
											 touch::pin::belt_right_back_ch};
	CoreTouchSensor _sensor_belt_right_front {_belt_right_front_input, _belt_right_front_pm, dac_touch_right,
											  touch::pin::belt_right_front_ch};

	uint16_t default_max_value_calib {0x0FFF};
	uint16_t default_min_value_calib {0x0000};

	struct State {
		bool ear_left_touched {false};
		bool ear_right_touched {false};
		bool belt_left_back_touched {false};
		bool belt_left_front_touched {false};
		bool belt_right_back_touched {false};
		bool belt_right_front_touched {false};
	};

	State _state;
	rtos::Thread _thread {};
};
}	// namespace leka
