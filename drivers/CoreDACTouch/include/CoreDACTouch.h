// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#ifndef _LEKA_OS_CORE_DAC_TOUCH_H_
#define _LEKA_OS_CORE_DAC_TOUCH_H_

#include "CoreI2C.h"

namespace leka {

class CoreDACTouch
{
  public:
	CoreDACTouch(interface::I2C &i2c, uint8_t address) : _i2c(i2c), _address(address) {};

  private:
	interface::I2C &_i2c;
	uint8_t _address;
};

}	// namespace leka

#endif	 // _LEKA_OS_CORE_DAC_TOUCH_H_
