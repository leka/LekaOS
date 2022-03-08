// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <cstdint>

#include "rtos/ThisThread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLTDC.hpp"
#include "CoreSTM32Hal.h"
#include "Graphics.h"

using namespace std::chrono_literals;

namespace leka {

class VideoKit
{
  public:
	explicit VideoKit(interface::STM32Hal &);

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

	void setFrameRateLimit(uint32_t framerate);

	void clear(gfx::Color color = gfx::Color::White);

	void draw(gfx::Drawable &drawable);

	void drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, gfx::Color color);

	void drawText(const char *text, uint32_t x, uint32_t y, gfx::Color color, gfx::Color bg = gfx::Color::Transparent);

	auto drawImage(interface::File &file, JPEGConfig *config = nullptr) -> uint32_t;

	auto drawImage(interface::File &file, JPEGConfig &config) -> uint32_t;

	void display();

  private:
	void refresh();
	void tick();

	interface::STM32Hal &_hal;

	// peripherals
	CoreJPEGModeDMA _corejpegmode;
	CoreJPEG _corejpeg;
	CoreDMA2D _coredma2d;

	// ltdc + dsi + lcd screen
	CoreLTDC _coreltdc;
	CoreDSI _coredsi;
	CoreLCDDriverOTM8009A _coreotm;
	CoreLCD _corelcd;

	rtos::Kernel::Clock::time_point _last_time {};
	std::chrono::milliseconds _frametime = 40ms;
};

}	// namespace leka
