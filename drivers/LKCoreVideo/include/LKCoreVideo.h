// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LKCOREVIDEO_H_
#define _LEKA_OS_DRIVER_LKCOREVIDEO_H_

#include "LKCoreDMA2DBase.h"
#include "LKCoreDSIBase.h"
#include "LKCoreFontBase.h"
#include "LKCoreGraphicsBase.h"
#include "LKCoreJPEGBase.h"
#include "LKCoreLCDBase.h"
#include "LKCoreLTDCBase.h"
#include "LKCoreSDRAM.h"
#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreVideo
{
  public:
	LKCoreVideo(LKCoreSTM32HalBase &hal, LKCoreSDRAM &coresdram, LKCoreDMA2DBase &coredma2d, LKCoreDSIBase &coredsi,
				LKCoreLTDCBase &coreltdc, LKCoreLCDBase &corelcd, LKCoreGraphicsBase &coregraphics,
				LKCoreFontBase &corefont, LKCoreJPEGBase &corejpeg);

	void initialize(void);

	void turnOff();
	void turnOn();

	void setBrightness(float value);

	void clearScreen(Color color = CGColor::white);
	void displayRectangle(LKCoreGraphicsBase::FilledRectangle rectangle, Color color);
	void displayImage(FIL *file);
	void displayText(const char *text, uint32_t size, uint32_t starting_line, Color foreground = CGColor::black,
					 Color background = CGColor::white);

  private:
	LKCoreSTM32HalBase &_hal;
	LKCoreSDRAM &_coresdram;
	LKCoreDMA2DBase &_coredma2d;
	LKCoreDSIBase &_coredsi;
	LKCoreLTDCBase &_coreltdc;
	LKCoreLCDBase &_corelcd;
	LKCoreGraphicsBase &_coregraphics;
	LKCoreFontBase &_corefont;
	LKCoreJPEGBase &_corejpeg;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LKCOREVIDEO_H_
