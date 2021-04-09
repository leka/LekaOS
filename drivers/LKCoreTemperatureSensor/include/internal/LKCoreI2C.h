// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_I2C_H_
#define _LEKA_OS_DRIVER_LK_CORE_I2C_H_

#include "I2C.h"

namespace leka {
namespace interface {
	class LKCoreI2C
	{
	  public:
		virtual ~LKCoreI2C()							= default;
		virtual int read(int, char *, int, bool)		= 0;
		virtual int write(int, const char *, int, bool) = 0;
	};
}	// namespace interface

class LKCoreI2C : public interface::LKCoreI2C
{
  public:
	explicit LKCoreI2C(mbed::I2C &i2c) : _i2c(i2c) {};
	virtual ~LKCoreI2C() = default;
	virtual int read(int, char *, int, bool) final;
	virtual int write(int, const char *, int, bool) final;

  private:
	mbed::I2C &_i2c;
};
}	// namespace leka
#endif	 //_LEKA_OS_DRIVER_LK_CORE_I2C_H_
