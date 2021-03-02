// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_SCREEN_H_
#define _LEKA_OS_DRIVER_SCREEN_H_

#include "drivers/PwmOut.h"

#include "LKCoreLCDDriverBase.h"

namespace leka {

class LKCoreLCD
{
  public:
	LKCoreLCD(LKCoreLCDDriverBase &driver);

	void initialize();

	void turnOn();
	void turnOff();

	void setBrightness(float value);

  private:
	LKCoreLCDDriverBase &_driver;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
