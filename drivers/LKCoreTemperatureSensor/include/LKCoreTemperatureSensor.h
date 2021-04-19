// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include <array>
#include <tuple>

#include "LKCoreI2C.h"
#include "LKUtils.h"
#include "hts221_reg.h"

namespace leka {

using celsius_t			 = float;
using relativeHumidity_t = float;

namespace interface {
	class LKCoreTemperatureSensor
	{
	  public:
		virtual int32_t init()					 = 0;
		virtual celsius_t getTemperature()		 = 0;
		virtual relativeHumidity_t getHumidity() = 0;
	};
}	// namespace interface

class LKCoreTemperatureSensor : public interface::LKCoreTemperatureSensor
{
  public:
	explicit LKCoreTemperatureSensor(interface::I2C &i2c);

	int32_t init() final;
	celsius_t getTemperature() final;
	relativeHumidity_t getHumidity() final;
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
