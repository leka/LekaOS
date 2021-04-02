// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_

#include "../../include/status.h"

//
// MARK: - Type definitions
//

using celsius_t = float_t;
using rH_t		= float_t;

class LKCoreTemperatureSensorDriverBase
{
  public:
	//
	// MARK: - Virtual methods
	//
	virtual uint8_t getId()			   = 0;
	virtual celsius_t getTemperature() = 0;
	virtual rH_t getHumidity()		   = 0;
};

#endif	 // _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_
