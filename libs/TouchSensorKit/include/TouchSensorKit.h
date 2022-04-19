// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "internal/TouchPinNames.h"

#include "CoreIOExpander.h"
#include "IOKit/DigitalIn.h"

namespace leka {

class TouchSensorKit
{
  public:
	explicit TouchSensorKit(leka::io::expanded::DigitalIn<> &pin) : _pin(pin) { _pin.mode(PinMode::PullUp); };

	auto isTouched() -> bool;
	auto setPin(leka::io::expanded::DigitalIn<> &pin) -> bool;

  private:
	leka::io::expanded::DigitalIn<> _pin;
};
}	// namespace leka
