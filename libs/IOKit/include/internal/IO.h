// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/IOExpander.h"

namespace leka::io::expanded::internal {

template <typename pin_underlying_type_t>
class IO
{
  public:
	IO(interface::IOExpander<pin_underlying_type_t> &expander, pin_underlying_type_t pin)
		: _expander(expander), _pin(pin)
	{
	}

	void setAsInput() { _expander.setPinAsInput(_pin); }

	void setMode(PinMode mode) { _expander.setModeForPin(_pin, mode); }

	auto readInput() -> int
	{
		auto val = _expander.readInputPin(_pin);
		return val;
	}

	void setAsOutput() { _expander.setPinAsOutput(_pin); }

	void writeOutput(int value) { _expander.writeOutputPin(_pin, value); }

	auto readOutput() -> int
	{
		auto val = _expander.readOutputPin(_pin);
		return val;
	}

  private:
	interface::IOExpander<pin_underlying_type_t> &_expander;
	pin_underlying_type_t _pin;
};

}	// namespace leka::io::expanded::internal
