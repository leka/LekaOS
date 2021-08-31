// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "interface/drivers/IOExpander.h"

namespace leka::io::expanded::internal {

template <typename PinType>
class IO
{
  public:
	IO(interface::IOExpander<PinType> &expander, PinType pin) : _expander(expander), _pin(pin) {}

	auto read() -> int
	{
		auto val = _expander.readInputPin(_pin);
		return val;
	}

	void setMode(PinMode mode) { _expander.setModeForPin(_pin, mode); }

	void setAsInput() { _expander.setPinAsInput(_pin); }

  private:
	interface::IOExpander<PinType> &_expander;
	PinType _pin;
};

}	// namespace leka::io::expanded::internal
