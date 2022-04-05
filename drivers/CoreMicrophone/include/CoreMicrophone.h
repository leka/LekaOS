// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/AnalogIn.h"

namespace leka {

class CoreMicrophone
{
  public:
	explicit CoreMicrophone(PinName pin) : _pin(mbed::AnalogIn(pin)) {}

	auto readVolume() -> float;

  private:
	mbed::AnalogIn _pin;
};

}	// namespace leka
