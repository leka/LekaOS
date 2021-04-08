// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_

#include "LKCoreHTS221Driver.h"
#include "LKCoreTemperatureSensorBase.h"

namespace leka {
class LKCoreTemperatureSensor : public LKCoreHTS221Driver, public LKCoreTemperatureSensorDriverBase
{
  public:
	explicit LKCoreTemperatureSensor(mbed::I2C &i2c);
	// virtual ~LKCoreTemperatureSensor() = default;

	// virtual float getTemperature();
  private:
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_
