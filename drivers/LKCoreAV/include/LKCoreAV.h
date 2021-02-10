// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LKCOREAV_H_
#define _LEKA_OS_DRIVER_LKCOREAV_H_

#include "LKScreen.h"
#include "dma2d.h"
#include "dsi.h"
#include "jpeg.h"
#include "lcd_properties.h"
#include "ltdc.h"
#include "st_sdram.h"

namespace leka {

class LKCoreAV
{
  public:
	LKCoreAV(PinName backlight);

	void initialize();
	static void initializeHAL();
	static void disposeHAL();

	uint16_t getWidth();
	uint16_t getHeight();
	void setBrightness(float value);
	void turnOff();
	void turnOn();

	void rotateUpsideDown(bool upside_down);
	void displayImage(FIL *file);

  private:
	LKScreen _screen;
	LKCoreLTDC _ltdc;
	LKCoreDSI _dsi;
	LKCoreDMA2D _dma2d;
	LKCoreJPEG _jpeg;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LKCOREAV_H_
