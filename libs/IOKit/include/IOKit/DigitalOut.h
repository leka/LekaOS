// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "internal/IO.h"

namespace leka::io::expanded {

template <typename pin_underlying_type_t = uint16_t>
class DigitalOut : public mbed::interface::DigitalOut, private internal::IO<pin_underlying_type_t>
{
  public:
	explicit DigitalOut(interface::IOExpander<pin_underlying_type_t> &expander, pin_underlying_type_t pin)
		: internal::IO<pin_underlying_type_t>(expander, pin)
	{
		internal::IO<pin_underlying_type_t>::setAsOutput();
	}

	~DigitalOut() override = default;

	void write(int value) override { internal::IO<pin_underlying_type_t>::write(value); }
	auto read() -> int override { return internal::IO<pin_underlying_type_t>::read(); }
	auto is_connected() -> int override { return 1; }
};

}	// namespace leka::io::expanded
