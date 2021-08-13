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
	IO(interface::IOExpander &parent, uint16_t pin) : _parent(parent), _pin(pin) {}

  protected:
	auto internalRead() -> int;
	void internalMode(PinMode mode);
	void internalInput();

  private:
	interface::IOExpander &_parent;
	uint16_t _pin;

	PlatformMutex _mutex;
};

}	// namespace leka::io::expanded::internal
