// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_IO_EXPANDER_H_
#define _LEKA_OS_INTERFACE_DRIVER_IO_EXPANDER_H_

#include <cstdint>

#include "PinNames.h"

namespace leka::interface {

template <typename PinType>
class IOExpander
{
  public:
	virtual ~IOExpander() = default;

	virtual void setPinAsInput(PinType pin) = 0;

	virtual auto readInputPin(PinType pin) -> int = 0;

	virtual void setModeForPin(PinType pin, PinMode mode) = 0;
	virtual auto getModeForPin(PinType pin) -> PinMode	  = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_IO_EXPANDER_H_
