// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "PinNamesTypes.h"

namespace leka::interface {

template <typename pin_underlying_type_t>
class IOExpander
{
  public:
	virtual ~IOExpander() = default;

	virtual void setPinAsInput(pin_underlying_type_t pin) = 0;

	virtual auto readInputPin(pin_underlying_type_t pin) -> int = 0;

	virtual void setModeForPin(pin_underlying_type_t pin, PinMode mode) = 0;
	virtual auto getModeForPin(pin_underlying_type_t pin) -> PinMode	= 0;
};

}	// namespace leka::interface
