// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_

#include "mbed.h"

#include "internal/status.h"

//
// MARK: - Type definitions
//

using mg_t = float;

class LKCoreTemperatureSensorDriverBase
{
  public:
	//
	// MARK: - Virtual methods
	//
	virtual status_t getId(uint8_t &id)		= 0;
	virtual status_t getTemperature(mg_t &) = 0;
	virtual status_t getHumidity(mg_t &)	= 0;
};

#endif	 // _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_DRIVER_BASE_H_
