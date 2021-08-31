// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LKCOREVIDEO_H_
#define _LEKA_OS_DRIVER_LKCOREVIDEO_H_

#include "LKCoreFontBase.h"
#include "LKCoreGraphicsBase.h"
#include "LKCoreJPEGBase.h"
#include "LKCoreSTM32HalBase.h"
#include "interface/CoreDMA2D.h"
#include "interface/CoreDSI.h"
#include "interface/CoreLCD.h"
#include "interface/CoreLTDC.h"
#include "interface/CoreSDRAM.h"

namespace leka {

class LKCoreVideo
{
  public:
	LKCoreVideo(LKCoreSTM32HalBase &hal, interface::CoreSDRAM &coresdram, interface::CoreDMA2D &coredma2d,
				interface::CoreDSI &coredsi, interface::CoreLTDC &coreltdc, interface::CoreLCD &corelcd,
				LKCoreGraphicsBase &coregraphics, LKCoreFontBase &corefont, LKCoreJPEGBase &corejpeg);

	void initialize(void);

	void turnOff();
	void turnOn();

	void setBrightness(float value);

	void clearScreen(CGColor color = CGColor::white);
	void displayRectangle(LKCoreGraphicsBase::FilledRectangle rectangle, CGColor color);
	void displayImage(FIL *file);
	void displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
					 CGColor background = CGColor::white);

  private:
	LKCoreSTM32HalBase &_hal;
	interface::CoreSDRAM &_coresdram;
	interface::CoreDMA2D &_coredma2d;
	interface::CoreDSI &_coredsi;
	interface::CoreLTDC &_coreltdc;
	interface::CoreLCD &_corelcd;
	LKCoreGraphicsBase &_coregraphics;
	LKCoreFontBase &_corefont;
	LKCoreJPEGBase &_corejpeg;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LKCOREVIDEO_H_
