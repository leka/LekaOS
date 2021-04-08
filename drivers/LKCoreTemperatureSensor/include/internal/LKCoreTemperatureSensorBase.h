// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_BASE_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_BASE_H_

namespace leka {

using celsius_t = float_t;

class LKCoreTemperatureSensorDriverBase
{
  public:
	explicit LKCoreTemperatureSensorDriverBase() = default;
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_BASE_H_
