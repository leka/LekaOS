// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_I2C_H_
#define _LEKA_OS_DRIVER_LK_CORE_I2C_H_

#include "drivers/I2C.h"

namespace leka::interface {

class I2C
{
  public:
	virtual ~I2C() = default;

	virtual int read(int address, char *data, int length, bool repeated)		= 0;
	virtual int write(int address, const char *data, int length, bool repeated) = 0;
};

}	// namespace leka::interface

namespace leka {

class LKCoreI2C : public interface::I2C
{
  public:
	explicit LKCoreI2C(mbed::I2C &i2c) : _i2c {i2c} {};

	int read(int address, char *data, int length, bool repeated) final;
	int write(int address, const char *data, int length, bool repeated) final;

  private:
	mbed::I2C &_i2c;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_I2C_H_
