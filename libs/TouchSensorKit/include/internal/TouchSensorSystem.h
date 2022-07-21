// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "CoreQDAC.h"
#include "DigitalOut.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"

namespace leka {

inline auto corei2c			  = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
inline auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
inline auto io_expander		  = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

inline auto ear_left_in			= io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
inline auto ear_right_in		= io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
inline auto belt_left_back_in	= io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
inline auto belt_left_front_in	= io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
inline auto belt_right_back_in	= io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
inline auto belt_right_front_in = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};

inline auto ear_left_pm			= io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
inline auto ear_right_pm		= io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
inline auto belt_left_back_pm	= io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
inline auto belt_left_front_pm	= io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
inline auto belt_right_back_pm	= io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA1};
inline auto belt_right_front_pm = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA0};

inline auto dac_touch_left	= CoreQDACMCP4728 {corei2c, 0xC0};
inline auto dac_touch_right = CoreQDACMCP4728 {corei2c, 0xC2};

inline auto sensor_ear_left		  = CoreTouchSensor {ear_left_in, ear_left_pm, dac_touch_left, Channel::C};
inline auto sensor_ear_right	  = CoreTouchSensor {ear_right_in, ear_right_pm, dac_touch_right, Channel::C};
inline auto sensor_belt_left_back = CoreTouchSensor {belt_left_back_in, belt_left_back_pm, dac_touch_left, Channel::B};
inline auto sensor_belt_left_front =
	CoreTouchSensor {belt_left_front_in, belt_left_front_pm, dac_touch_left, Channel::A};
inline auto sensor_belt_right_back =
	CoreTouchSensor {belt_right_back_in, belt_right_back_pm, dac_touch_right, Channel::B};
inline auto sensor_belt_right_front =
	CoreTouchSensor {belt_right_front_in, belt_right_front_pm, dac_touch_right, Channel::A};

}	// namespace leka
