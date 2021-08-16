// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// * Project based on work by @AGlass0fMilk
// * https://github.com/AGlass0fMilk/ep-oc-mcu/blob/refine-io-drivers/devices/MCP23008/

#include "drivers/interfaces/InterfaceDigitalIn.h"

#include "internal/IO.h"

namespace leka::io::expanded {

template <typename PinType = uint16_t>
class DigitalIn : public mbed::interface::DigitalIn, private internal::IO<PinType>
{
  public:
	explicit DigitalIn(interface::IOExpander<PinType> &expander, PinType pin) : internal::IO<PinType>(expander, pin)
	{
		internal::IO<PinType>::setAsInput();
	}
	~DigitalIn() override = default;

	auto read() -> int override { return internal::IO<PinType>::read(); }
	void mode(::PinMode mode) override { internal::IO<PinType>::setMode(mode); }
	auto is_connected() -> int override { return 1; }
};

}	// namespace leka::io::expanded
