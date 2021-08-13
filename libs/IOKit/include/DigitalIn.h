// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// * Project based on work by @AGlass0fMilk
// * https://github.com/AGlass0fMilk/ep-oc-mcu/blob/refine-io-drivers/devices/MCP23008/

#include "drivers/interfaces/InterfaceDigitalIn.h"

#include "internal/IO.h"

namespace leka::io::expanded {

class DigitalIn : public mbed::interface::DigitalIn, private internal::IO
{
	using IO = internal::IO;

  public:
	explicit DigitalIn(interface::IOExpander &parent, uint16_t pin);
	~DigitalIn() override = default;
	auto read() -> int override;
	void mode(PinMode mode) override;
	auto is_connected() -> int override;
};

}	// namespace leka::io::expanded
