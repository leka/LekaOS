// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreIOExpander.h"

namespace leka::touch::pin {

constexpr inline uint16_t ear_left		   = leka::mcp23017::pin::PB5;
constexpr inline uint16_t ear_right		   = leka::mcp23017::pin::PB4;
constexpr inline uint16_t belt_left_back   = leka::mcp23017::pin::PB3;
constexpr inline uint16_t belt_left_front  = leka::mcp23017::pin::PB2;
constexpr inline uint16_t belt_right_back  = leka::mcp23017::pin::PB1;
constexpr inline uint16_t belt_right_front = leka::mcp23017::pin::PB0;
constexpr inline uint16_t all =
	ear_left + ear_right + belt_left_back + belt_left_front + belt_right_back + belt_right_front;

}	// namespace leka::touch::pin
