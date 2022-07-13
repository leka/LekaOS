// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreIOExpander.h"

namespace leka::touch {

namespace pin {
	inline constexpr auto ear_left_input		 = uint16_t {leka::mcp23017::pin::PB5};
	inline constexpr auto ear_right_input		 = uint16_t {leka::mcp23017::pin::PB4};
	inline constexpr auto belt_left_back_input	 = uint16_t {leka::mcp23017::pin::PB3};
	inline constexpr auto belt_left_front_input	 = uint16_t {leka::mcp23017::pin::PB2};
	inline constexpr auto belt_right_back_input	 = uint16_t {leka::mcp23017::pin::PB1};
	inline constexpr auto belt_right_front_input = uint16_t {leka::mcp23017::pin::PB0};

	inline constexpr auto ear_left_power_mode		  = uint16_t {leka::mcp23017::pin::PA5};
	inline constexpr auto ear_right_power_mode		  = uint16_t {leka::mcp23017::pin::PA4};
	inline constexpr auto belt_left_back_power_mode	  = uint16_t {leka::mcp23017::pin::PA3};
	inline constexpr auto belt_left_front_power_mode  = uint16_t {leka::mcp23017::pin::PA2};
	inline constexpr auto belt_right_back_power_mode  = uint16_t {leka::mcp23017::pin::PA1};
	inline constexpr auto belt_right_front_power_mode = uint16_t {leka::mcp23017::pin::PA0};
}	// namespace pin

namespace power_mode {
	inline constexpr auto low	 = uint16_t {0x00};
	inline constexpr auto normal = uint16_t {0x01};

}	// namespace power_mode

}	// namespace leka::touch
