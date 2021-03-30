// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_BATTERY_H_
#define _LEKA_OS_DRIVER_LK_CORE_BATTERY_H_

#include "drivers/AnalogIn.h"

namespace leka {

class LKCoreBattery
{
  public:
	LKCoreBattery(PinName pin) : _pin {mbed::AnalogIn(pin)} {};

	float readVoltage(void);

  private:
	mbed::AnalogIn _pin;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_BATTERY_H_
