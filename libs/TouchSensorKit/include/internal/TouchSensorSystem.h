// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreIOExpander.h"

namespace leka::touch {

namespace pin {
	constexpr inline uint16_t ear_left_input		 = leka::mcp23017::pin::PB5;
	constexpr inline uint16_t ear_right_input		 = leka::mcp23017::pin::PB4;
	constexpr inline uint16_t belt_left_back_input	 = leka::mcp23017::pin::PB3;
	constexpr inline uint16_t belt_left_front_input	 = leka::mcp23017::pin::PB2;
	constexpr inline uint16_t belt_right_back_input	 = leka::mcp23017::pin::PB1;
	constexpr inline uint16_t belt_right_front_input = leka::mcp23017::pin::PB0;

	constexpr inline uint16_t ear_left_power_mode		  = leka::mcp23017::pin::PA5;
	constexpr inline uint16_t ear_right_power_mode		  = leka::mcp23017::pin::PA4;
	constexpr inline uint16_t belt_left_back_power_mode	  = leka::mcp23017::pin::PA3;
	constexpr inline uint16_t belt_left_front_power_mode  = leka::mcp23017::pin::PA2;
	constexpr inline uint16_t belt_right_back_power_mode  = leka::mcp23017::pin::PA1;
	constexpr inline uint16_t belt_right_front_power_mode = leka::mcp23017::pin::PA0;
}	// namespace pin

namespace power_mode {
	constexpr inline uint16_t low	 = 0x00;
	constexpr inline uint16_t normal = 0X01;

}	// namespace power_mode

}	// namespace leka::touch
