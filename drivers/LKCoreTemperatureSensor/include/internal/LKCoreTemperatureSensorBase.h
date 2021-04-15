// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_

#include <stdint.h>

using celsius_t			 = float;
using relativeHumidity_t = float;

class LKCoreTemperatureSensorDriverBase
{
  public:
	virtual bool init()						 = 0;
	virtual celsius_t getTemperature()		 = 0;
	virtual relativeHumidity_t getHumidity() = 0;
};

#endif	 // _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_
