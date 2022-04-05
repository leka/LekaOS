// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/AnalogIn.h"

namespace leka {

class CoreLightSensor
{
  public:
	enum class LuminosityLevel
	{
		dark,
		shadow,
		ambient,
		sunny
	};

	explicit CoreLightSensor(PinName pin) : _pin(mbed::AnalogIn(pin))
	{
		// nothing do to
	}

	auto readLuminosity() -> float;

	auto getLuminosityLevel() -> LuminosityLevel;

  private:
	auto readRawValue() -> float;

	mbed::AnalogIn _pin;
};

}	// namespace leka
