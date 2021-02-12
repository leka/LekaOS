// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_SCREEN_H_
#define _LEKA_OS_DRIVER_SCREEN_H_

#include "drivers/PwmOut.h"

namespace leka {

class LKCoreLCD
{
  public:
	LKCoreLCD(PinName backlight, float brightness = 0.10f);

	void initialize();

	void turnOn();
	void turnOff();

	void setBrightness(float value);

  private:
	mbed::PwmOut _brightness;

	void setLandscapeOrientation();
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
