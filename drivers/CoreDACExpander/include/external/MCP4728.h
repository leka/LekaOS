// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::mcp4728 {

namespace registers {

	constexpr inline uint8_t vref		= 0x80;
	constexpr inline uint8_t power_down = 0xA0;
	constexpr inline uint8_t gain		= 0xC0;

}	// namespace registers

}	// namespace leka::mcp4728
