// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreJPEG.hpp"
#include "interface/DMA2D.hpp"
#include "interface/DSI.hpp"
#include "interface/Font.hpp"
#include "interface/Graphics.hpp"
#include "interface/JPEG.hpp"
#include "interface/LCD.hpp"
#include "interface/LTDC.hpp"
#include "interface/SDRAM.hpp"
#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"

namespace leka {

class CoreVideo
{
  public:
	CoreVideo(interface::STM32Hal &hal, interface::SDRAM &coresdram, interface::DMA2DBase &coredma2d,
			  interface::DSIBase &coredsi, interface::LTDCBase &coreltdc, interface::LCD &corelcd,
			  interface::Graphics &coregraphics, interface::Font &corefont, interface::JPEGBase &corejpeg);

	void initialize();

	void turnOff();
	void turnOn();

	void setBrightness(float value);

	void clearScreen(CGColor color = CGColor::white);
	void displayRectangle(interface::Graphics::FilledRectangle rectangle, CGColor color);
	void displayImage(interface::File &file);
	void displayVideo(interface::File &file);
	void displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
					 CGColor background = CGColor::white);
	void display();

  private:
	interface::STM32Hal &_hal;
	interface::SDRAM &_coresdram;
	interface::DMA2DBase &_coredma2d;
	interface::LTDCBase &_coreltdc;
	interface::DSIBase &_coredsi;
	interface::LCD &_corelcd;
	interface::Graphics &_coregraphics;
	interface::Font &_corefont;
	interface::JPEGBase &_corejpeg;
};

}	// namespace leka
