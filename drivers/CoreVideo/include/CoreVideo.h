// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CoreVideo_H_
#define _LEKA_OS_DRIVER_CoreVideo_H_

#include "LKCoreFatFs.h"
#include "CoreJPEG.h"
#include "LKCoreSTM32HalBase.h"
#include "interface/CoreDMA2D.h"
#include "interface/CoreDSI.h"
#include "interface/CoreFont.h"
#include "interface/CoreGraphics.h"
#include "interface/CoreJPEG.h"
#include "interface/CoreLCD.h"
#include "interface/CoreLTDC.h"
#include "interface/CoreSDRAM.h"

namespace leka {

class CoreVideo
{
  public:
	CoreVideo(LKCoreSTM32HalBase &hal, interface::CoreSDRAM &coresdram, interface::CoreDMA2D &coredma2d,
				interface::CoreDSI &coredsi, interface::CoreLTDC &coreltdc, interface::CoreLCD &corelcd,
				interface::CoreGraphics &coregraphics, interface::CoreFont &corefont,
				interface::CoreJPEG &corejpeg);

	void initialize();

	void turnOff();
	void turnOn();

	void setBrightness(float value);

	void clearScreen(CGColor color = CGColor::white);
	void displayRectangle(interface::CoreGraphics::FilledRectangle rectangle, CGColor color);
	void displayImage(LKCoreFatFs &file);
	void displayVideo(LKCoreFatFs &file);
	void displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
					 CGColor background = CGColor::white);
	void display();

  private:
	LKCoreSTM32HalBase &_hal;
	interface::CoreSDRAM &_coresdram;
	interface::CoreDMA2D &_coredma2d;
	interface::CoreLTDC &_coreltdc;
	interface::CoreDSI &_coredsi;
	interface::CoreLCD &_corelcd;
	interface::CoreGraphics &_coregraphics;
	interface::CoreFont &_corefont;
	interface::CoreJPEG &_corejpeg;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CoreVideo_H_
