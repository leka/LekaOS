// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_SCREEN_H_
#define _LEKA_OS_DRIVER_SCREEN_H_

#include "drivers/PwmOut.h"

#include "LKCoreDSIBase.h"

namespace leka {

class LKCoreLCD
{
  public:
	LKCoreLCD(LKCoreDSIBase &dsi, PinName backlight, float brightness = 0.10f);

	void initialize();

	void turnOn();
	void turnOff();

	void setBrightness(float value);

	uint8_t getCommandByteForLandscapeOrientation(bool vertical_symmetry, bool horizontal_symmetry, bool is_landscape,
												  bool reverse_refresh_top_to_bottom, bool use_bgr);
	void setLandscapeOrientation(bool vertical_symmetry = true, bool horizontal_symmetry = false,
								 bool is_landscape = true, bool reverse_refresh_top_to_bottom = false,
								 bool use_bgr = false);

  private:
	LKCoreDSIBase &_dsi;
	mbed::PwmOut _brightness;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
