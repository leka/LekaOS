// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include "../external/include/HTS221_reg.h"
#include "I2C.h"

namespace leka {

class LKCoreHTS221Driver
{
  public:
	explicit LKCoreHTS221Driver(mbed::I2C &i2c) : _i2c {i2c} {};

  private:
	mbed::I2C &_i2c;
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
