// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "platform/PlatformMutex.h"

#include "interface/drivers/IOExpander.h"
#include "targets/TARGET_STM/PinNamesTypes.h"

namespace leka::io::expanded::internal {

class IO
{
  public:
	IO(interface::IOExpander &expander, uint16_t pin) : expander(expander), _pin(pin) {}

	auto read() -> int;
	void setMode(PinMode mode);
	void setAsInput();

  private:
	interface::IOExpander &expander;
	uint16_t _pin;

	PlatformMutex _mutex;
};

}	// namespace leka::io::expanded::internal
