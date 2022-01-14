// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORELED_H_
#define _LEKA_OS_DRIVER_CORELED_H_

#include "drivers/SPI.h"


namespace leka {

class CoreLED
{
  public:
	CoreLED();



  private:

	mbed::SPI &mSPI;

};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORELED_H_
