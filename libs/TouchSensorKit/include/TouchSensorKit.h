// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreDACTouch.h"
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

	void updateState();
	void printState();

	void resetByPowerMode();

	void adjust_sensivity(uint16_t value);
	void calibration();

  private:
	void setPullMode(PinMode mode);

	void setPowerMode(int power_mode);

	void initDACTouch();

	void calibrateTwoSensors(bool &sensor_left, bool &sensor_right, uint8_t channel);
	void calibrateEars();
	void calibrateBeltLBRF();
	void calibrateBeltRBLF();

	CoreI2C corei2c {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
	mbed::DigitalOut io_expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	CoreIOExpanderMCP23017 io_expander {corei2c, io_expander_reset};
	CoreDACTouchMCP4728 dac_touch_left {corei2c, 0xC0};
	CoreDACTouchMCP4728 dac_touch_right {corei2c, 0xC2};

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

	bool _ear_left_touched {false};
	bool _ear_right_touched {false};
	bool _belt_left_back_touched {false};
	bool _belt_left_front_touched {false};
	bool _belt_right_back_touched {false};
	bool _belt_right_front_touched {false};

	uint16_t default_max_value_calib {0x0FFF};
	uint16_t default_min_value_calib {0x0000};
};
}	// namespace leka
