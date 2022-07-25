// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "CoreQDAC.h"
#include "CoreTouchSensor.h"
#include "DigitalOut.h"
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"

namespace leka {

inline auto corei2c			  = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
inline auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
inline auto io_expander		  = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

namespace detect_pin {
	inline auto ear_left		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
	inline auto ear_right		 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB4};
	inline auto belt_left_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB3};
	inline auto belt_left_front	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB2};
	inline auto belt_right_back	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB1};
	inline auto belt_right_front = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB0};
}	// namespace detect_pin

namespace power_mode_pin {
	inline auto ear_left		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};
	inline auto ear_right		 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA4};
	inline auto belt_left_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA3};
	inline auto belt_left_front	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA2};
	inline auto belt_right_back	 = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA1};
	inline auto belt_right_front = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA0};
}	// namespace power_mode_pin

namespace dac {
	inline auto left  = CoreQDACMCP4728 {corei2c, 0xC0};
	inline auto right = CoreQDACMCP4728 {corei2c, 0xC2};
	namespace channel {
		inline auto ear_left		 = Channel::C;
		inline auto ear_right		 = Channel::C;
		inline auto belt_left_back	 = Channel::B;
		inline auto belt_left_front	 = Channel::A;
		inline auto belt_right_back	 = Channel::B;
		inline auto belt_right_front = Channel::A;
	}	// namespace channel
}	// namespace dac

namespace sensor {
	inline auto ear_left =
		CoreTouchSensor {detect_pin::ear_left, power_mode_pin::ear_left, dac::left, dac::channel::ear_left};
	inline auto ear_right =
		CoreTouchSensor {detect_pin::ear_right, power_mode_pin::ear_right, dac::right, dac::channel::ear_right};
	inline auto belt_left_back = CoreTouchSensor {detect_pin::belt_left_back, power_mode_pin::belt_left_back, dac::left,
												  dac::channel::belt_left_back};
	inline auto belt_left_front	 = CoreTouchSensor {detect_pin::belt_left_front, power_mode_pin::belt_left_front,
													dac::left, dac::channel::belt_left_front};
	inline auto belt_right_back	 = CoreTouchSensor {detect_pin::belt_right_back, power_mode_pin::belt_right_back,
													dac::right, dac::channel::belt_right_back};
	inline auto belt_right_front = CoreTouchSensor {detect_pin::belt_right_front, power_mode_pin::belt_right_front,
													dac::right, dac::channel::belt_right_front};
}	// namespace sensor

namespace position {
	inline constexpr auto ear_left		   = uint8_t {0};
	inline constexpr auto ear_right		   = uint8_t {1};
	inline constexpr auto belt_left_back   = uint8_t {2};
	inline constexpr auto belt_left_front  = uint8_t {3};
	inline constexpr auto belt_right_back  = uint8_t {4};
	inline constexpr auto belt_right_front = uint8_t {5};
}	// namespace position

}	// namespace leka
