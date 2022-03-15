// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreJPEG.hpp"
#include "interface/DMA2D.hpp"
#include "interface/DSI.hpp"
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
	CoreVideo(interface::STM32Hal &hal, interface::LCD &corelcd, interface::DSIBase &coredsi,
			  interface::LTDCBase &coreltdc, interface::DMA2DBase &coredma2d, interface::JPEGBase &corejpeg,
			  interface::SDRAM &coresdram);

	void initialize();

	void turnOff();
	void turnOn();

	void setBrightness(float value);

	void displayImage(interface::File &file);
	void displayVideo(interface::File &file);
	void display();

  private:
	interface::STM32Hal &_hal;

	interface::LCD &_corelcd;
	interface::DSIBase &_coredsi;
	interface::LTDCBase &_coreltdc;
	interface::DMA2DBase &_coredma2d;
	interface::JPEGBase &_corejpeg;
	interface::SDRAM &_coresdram;
};

}	// namespace leka
