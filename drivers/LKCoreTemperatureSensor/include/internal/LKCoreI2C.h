// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_I2C_H_
#define _LEKA_OS_DRIVER_LK_CORE_I2C_H_

#include "I2C.h"

namespace leka::interface{
class LKCoreI2C
{
  public:
	explicit LKCoreI2C(mbed::I2C &i2c) : _i2c(i2c) {};
	virtual ~LKCoreI2C() = default;
	virtual int read(int, char *, int, bool);
	virtual int write(int, const char *, int, bool);

  private:
	mbed::I2C &_i2c;
};
} // namespace leka::interface
#endif	 //_LEKA_OS_DRIVER_LK_CORE_I2C_H_
