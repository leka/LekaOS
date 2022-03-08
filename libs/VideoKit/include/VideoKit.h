// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "rtos/ThisThread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLTDC.hpp"
#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "Graphics.h"

using namespace std::chrono_literals;

namespace leka {

class VideoKit
{
  public:
	explicit VideoKit();

	void initializeScreen() {};

	void turnOn() {};
	void turnOff() {};

	void displayImage(const char *path) {};

	void playVideo(const char *path, bool must_loop = false) {};
	void stopVideo() {};

	auto getDSI() -> CoreDSI &;
	auto getLTDC() -> CoreLTDC &;
	auto getDMA2D() -> CoreDMA2D &;
	auto getJPEG() -> CoreJPEG &;

	void initialize();

	void setFrameRateLimit(unsigned framerate);

	void clear(gfx::Color color = gfx::Color::White);

	void draw(gfx::Drawable &drawable);
	void drawRectangle(gfx::Rectangle rect, uint32_t x, uint32_t y);

	// temporary
	void drawImage(interface::File &file);
	void drawVideo(interface::File &file);

	void display();

  private:
	void refresh();
	void tick();

	CoreSTM32Hal _hal;
	CoreSDRAM _coresdram;

	// peripherals
	CoreJPEG _corejpeg;
	CoreDMA2D _coredma2d;

	// ltdc + dsi + lcd screen
	CoreLTDC _coreltdc;
	CoreDSI _coredsi;
	CoreLCDDriverOTM8009A _coreotm;
	CoreLCD _corelcd;

	rtos::Kernel::Clock::time_point _last_time = rtos::Kernel::Clock::now();
	std::chrono::milliseconds _frametime	   = 40ms;
};

}	// namespace leka
