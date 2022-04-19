// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::mcp23017 {

namespace pin {

	constexpr inline uint16_t PA0 = 0x0001;
	constexpr inline uint16_t PA1 = 0x0002;
	constexpr inline uint16_t PA2 = 0x0004;
	constexpr inline uint16_t PA3 = 0x0008;
	constexpr inline uint16_t PA4 = 0x0010;
	constexpr inline uint16_t PA5 = 0x0020;
	constexpr inline uint16_t PA6 = 0x0040;
	constexpr inline uint16_t PA7 = 0x0080;

	constexpr inline uint16_t PB0 = 0x0100;
	constexpr inline uint16_t PB1 = 0x0200;
	constexpr inline uint16_t PB2 = 0x0400;
	constexpr inline uint16_t PB3 = 0x0800;
	constexpr inline uint16_t PB4 = 0x1000;
	constexpr inline uint16_t PB5 = 0x2000;
	constexpr inline uint16_t PB6 = 0x4000;
	constexpr inline uint16_t PB7 = 0x8000;
	constexpr inline uint16_t All = 0xFFFF;

}	// namespace pin

namespace registers {

	constexpr inline uint8_t IODIR	 = 0x00;
	constexpr inline uint8_t IPOL	 = 0x02;
	constexpr inline uint8_t GPINTEN = 0x04;
	constexpr inline uint8_t DEFVAL	 = 0x06;
	constexpr inline uint8_t INTCON	 = 0x08;
	constexpr inline uint8_t IOCON	 = 0x0A;
	constexpr inline uint8_t GPPU	 = 0x0C;
	constexpr inline uint8_t INTF	 = 0x0E;
	constexpr inline uint8_t INTCAP	 = 0x10;
	constexpr inline uint8_t GPIO	 = 0x12;
	constexpr inline uint8_t OLAT	 = 0x14;

}	// namespace registers

// namespace register

namespace directory {

	constexpr inline uint8_t DIR_OUTPUT = 0x00;
	constexpr inline uint8_t DIR_INPUT	= 0x01;

}	// namespace directory

}	// namespace leka::mcp23017
