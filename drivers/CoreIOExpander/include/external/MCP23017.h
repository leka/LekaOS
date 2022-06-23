// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::mcp23017 {

namespace pin {

	inline constexpr auto PA0 = uint16_t {0x0001};
	inline constexpr auto PA1 = uint16_t {0x0002};
	inline constexpr auto PA2 = uint16_t {0x0004};
	inline constexpr auto PA3 = uint16_t {0x0008};
	inline constexpr auto PA4 = uint16_t {0x0010};
	inline constexpr auto PA5 = uint16_t {0x0020};
	inline constexpr auto PA6 = uint16_t {0x0040};
	inline constexpr auto PA7 = uint16_t {0x0080};

	inline constexpr auto PB0 = uint16_t {0x0100};
	inline constexpr auto PB1 = uint16_t {0x0200};
	inline constexpr auto PB2 = uint16_t {0x0400};
	inline constexpr auto PB3 = uint16_t {0x0800};
	inline constexpr auto PB4 = uint16_t {0x1000};
	inline constexpr auto PB5 = uint16_t {0x2000};
	inline constexpr auto PB6 = uint16_t {0x4000};
	inline constexpr auto PB7 = uint16_t {0x8000};
	inline constexpr auto All = uint16_t {0xFFFF};

}	// namespace pin

namespace internal_register {

	inline constexpr auto IODIR	  = uint8_t {0x00};
	inline constexpr auto IPOL	  = uint8_t {0x02};
	inline constexpr auto GPINTEN = uint8_t {0x04};
	inline constexpr auto DEFVAL  = uint8_t {0x06};
	inline constexpr auto INTCON  = uint8_t {0x08};
	inline constexpr auto IOCON	  = uint8_t {0x0A};
	inline constexpr auto GPPU	  = uint8_t {0x0C};
	inline constexpr auto INTF	  = uint8_t {0x0E};
	inline constexpr auto INTCAP  = uint8_t {0x10};
	inline constexpr auto GPIO	  = uint8_t {0x12};
	inline constexpr auto OLAT	  = uint8_t {0x14};

}	// namespace internal_register

namespace level {

	inline constexpr auto low  = uint8_t {0x00};
	inline constexpr auto high = uint8_t {0x01};

}	// namespace level

}	// namespace leka::mcp23017
