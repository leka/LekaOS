// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_LIGHT_SENSOR_H_
#define _LEKA_OS_DRIVER_LK_CORE_LIGHT_SENSOR_H_

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

	explicit CoreLightSensor(PinName pin) : _pin {mbed::AnalogIn(pin)} {};

	auto readLuminosity() -> float;

	auto getLuminosityLevel() -> LuminosityLevel;

  private:
	auto readRawValue() -> float;

	mbed::AnalogIn _pin;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_LIGHT_SENSOR_H_
