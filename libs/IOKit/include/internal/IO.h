// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_IO_KIT_INTERNAL_IO_H_
#define _LEKA_OS_LIB_IO_KIT_INTERNAL_IO_H_

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

	auto read() -> int
	{
		auto val = _expander.readInputPin(_pin);
		return val;
	}

	void setMode(PinMode mode) { _expander.setModeForPin(_pin, mode); }

	void setAsInput() { _expander.setPinAsInput(_pin); }

  private:
	interface::IOExpander<pin_underlying_type_t> &_expander;
	pin_underlying_type_t _pin;
};

}	// namespace leka::io::expanded::internal

#endif	 // _LEKA_OS_LIB_IO_KIT_INTERNAL_IO_H_
