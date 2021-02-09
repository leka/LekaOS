// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_SCREEN_H_
#define _LEKA_OS_DRIVER_SCREEN_H_

#include "drivers/PwmOut.h"

#include "st_otm8009a.h"

namespace leka {

class LKScreen
{
  public:
	LKScreen(PinName backlight);

	void initializeDriver();

	void setBrightness(float value);
	void turnOff();
	void turnOn();

	void rotateUpsideDown(bool upside_down);

  private:
	mbed::PwmOut _brightness;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
