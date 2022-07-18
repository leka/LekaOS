// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace leka {

enum class Channel : uint8_t
{
	A,
	B,
	C,
	D
};

namespace interface {
	class QDAC
	{
	  public:
		virtual ~QDAC() = default;

		virtual void init()														= 0;
		virtual void write(Channel channel, uint16_t data, bool eeprom = false) = 0;
		virtual auto read(Channel channel, bool eeprom = false) -> uint16_t		= 0;
	};

}	// namespace interface
}	// namespace leka
