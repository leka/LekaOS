// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LKCOREAV_H_
#define _LEKA_OS_DRIVER_LKCOREAV_H_

#include "LKScreen.h"
#include "dma2d.h"
#include "dsi.h"
#include "jpeg.h"
#include "ltdc.h"
#include "sdram.h"

namespace leka {

class LKCoreAV
{
  public:
	LKCoreAV(LCD_Model lcd_model);
	LKCoreAV(LKScreen &screen);

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
	void JPEGCodecInit();

	static void DMAInit();
	void DMA2DInit();

  private:
	LKScreen _screen;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LKCOREAV_H_
