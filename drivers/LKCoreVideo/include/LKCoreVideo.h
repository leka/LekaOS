// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LKCOREVIDEO_H_
#define _LEKA_OS_DRIVER_LKCOREVIDEO_H_

#include "interface/LKCoreDMA2D.h"
#include "interface/LKCoreDSI.h"
#include "LKCoreFatFs.h"
#include "interface/LKCoreFont.h"
#include "interface/LKCoreGraphics.h"
#include "LKCoreJPEG.h"
#include "interface/LKCoreJPEG.h"
#include "interface/LKCoreLCD.h"
#include "interface/LKCoreLTDC.h"
#include "interface/LKCoreSDRAM.h"
#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreVideo
{
  public:
	LKCoreVideo(LKCoreSTM32HalBase &hal, interface::LKCoreSDRAM &coresdram, interface::LKCoreDMA2D &coredma2d, interface::LKCoreDSI &coredsi,
				interface::LKCoreLTDC &coreltdc, interface::LKCoreLCD &corelcd, interface::LKCoreGraphics &coregraphics,
				interface::LKCoreFont &corefont, interface::LKCoreJPEG &corejpeg);

	void initialize();

	void turnOff();
	void turnOn();

	void setBrightness(float value);

	void clearScreen(CGColor color = CGColor::white);
	void displayRectangle(interface::LKCoreGraphics::FilledRectangle rectangle, CGColor color);
	void displayImage(LKCoreFatFs &file);
	void displayVideo(LKCoreFatFs &file);
	void displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
					 CGColor background = CGColor::white);
	void display();

  private:
	LKCoreSTM32HalBase &_hal;
	interface::LKCoreSDRAM &_coresdram;
	interface::LKCoreDMA2D &_coredma2d;
	interface::LKCoreLTDC &_coreltdc;
	interface::LKCoreDSI &_coredsi;
	interface::LKCoreLCD &_corelcd;
	interface::LKCoreGraphics &_coregraphics;
	interface::LKCoreFont &_corefont;
	interface::LKCoreJPEG &_corejpeg;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_LKCOREVIDEO_H_
