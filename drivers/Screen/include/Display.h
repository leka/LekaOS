// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DISPLAY_H_
#define _LEKA_OS_DRIVER_DISPLAY_H_

#include "Screen.h"
#include "dsi.h"
#include "ltdc.h"
#include "sdram.h"

namespace leka {

class Display
{
  public:
	Display(LKScreen &screen);

	uint16_t getWidth();
	uint16_t getHeight();
	void setBrightness(float value);
	void turnOff();
	void turnOn();

	void rotateUpsideDown(bool upside_down);

	void Init();
	static void MSPInit();
	static void MSPDeInit();
	void DSIInit();
	void LTDCInit();
	static void SDRAMInit();
	void LCDDriverInit();
	static void JPEGCodecInit();

	static void DMAInit();
	static void DMA2DInit();

  private:
	LKScreen &_screen;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_DISPLAY_H_
