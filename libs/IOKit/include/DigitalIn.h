// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// * Project based on work by @AGlass0fMilk
// * https://github.com/AGlass0fMilk/ep-oc-mcu/blob/refine-io-drivers/devices/MCP23008/

#pragma once

#include "drivers/interfaces/InterfaceDigitalIn.h"

#include "internal/IO.h"

namespace leka::io::expanded {

template <typename pin_underlying_type_t = uint16_t>
class DigitalIn : public mbed::interface::DigitalIn, private internal::IO<pin_underlying_type_t>
{
  public:
	explicit DigitalIn(interface::IOExpander<pin_underlying_type_t> &expander, pin_underlying_type_t pin)
		: internal::IO<pin_underlying_type_t>(expander, pin)
	{
		internal::IO<pin_underlying_type_t>::setAsInput();
	}

	~DigitalIn() override = default;

	auto read() -> int override { return internal::IO<pin_underlying_type_t>::read(); }
	void mode(::PinMode mode) override { internal::IO<pin_underlying_type_t>::setMode(mode); }
	auto is_connected() -> int override { return 1; }
};

}	// namespace leka::io::expanded
