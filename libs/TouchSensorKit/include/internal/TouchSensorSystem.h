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

auto corei2c		   = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
auto io_expander	   = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

auto ear_left_in		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
auto ear_right_in		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
auto belt_left_back_in	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
auto belt_left_front_in	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
auto belt_right_back_in	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
auto belt_right_front_in = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};

auto ear_left_pm		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
auto ear_right_pm		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
auto belt_left_back_pm	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
auto belt_left_front_pm	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
auto belt_right_back_pm	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA1};
auto belt_right_front_pm = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA0};

auto dac_touch_left	 = CoreQDACMCP4728 {corei2c, 0xC0};
auto dac_touch_right = CoreQDACMCP4728 {corei2c, 0xC2};

auto ear_left_ch		 = Channel::A;
auto ear_right_ch		 = Channel::A;
auto belt_left_back_ch	 = Channel::B;
auto belt_left_front_ch	 = Channel::B;
auto belt_right_back_ch	 = Channel::C;
auto belt_right_front_ch = Channel::C;

}	// namespace leka
