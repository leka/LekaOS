// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <cstdint>

#include "rtos/EventFlags.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLTDC.hpp"
#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "FileSystemKit.h"
#include "Graphics.h"

namespace leka {

class VideoKit
{
  public:
	explicit VideoKit(interface::STM32Hal &);

	void initializeScreen();

	void turnOn();
	void turnOff();

	void displayImage(const char *path);

	void playVideo(const char *path, bool must_loop = false);
	void stopVideo();

	auto getDSI() -> CoreDSI &;
	auto getLTDC() -> CoreLTDC &;
	auto getDMA2D() -> CoreDMA2D &;
	auto getJPEG() -> CoreJPEG &;

	void initialize();

	void setBrightness(float value);

	void setFrameRateLimit(uint32_t framerate);

	void fillJPEGConfig(interface::File &file, JPEGConfig *config);

	void clear(gfx::Color color = gfx::Color::White);

	void draw(gfx::Drawable &drawable);

	void drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, gfx::Color color);

	void drawText(const char *text, uint32_t x, uint32_t y, gfx::Color color, gfx::Color bg = gfx::Color::Transparent);

	auto drawImage(interface::File &file) -> uint32_t;

	auto drawImage(interface::File &file, JPEGConfig &config) -> uint32_t;

	void display();

	void resetCounters();
	void displayCounters();

  private:
	void refresh();
	void tick();

	void runVideo();

	interface::STM32Hal &_hal;

	// peripherals
	CoreSDRAM _coresdram;
	CoreJPEGModeDMA _corejpegmode;
	CoreJPEG _corejpeg;
	CoreDMA2D _coredma2d;

	// ltdc + dsi + lcd screen
	CoreLTDC _coreltdc;
	CoreDSI _coredsi;
	CoreLCDDriverOTM8009A _coreotm;
	CoreLCD _corelcd;

	rtos::Kernel::Clock::time_point _last_time {};
	std::chrono::milliseconds _frametime;

	std::chrono::milliseconds _whole_duration;
	uint32_t _whole_tick;
	std::chrono::milliseconds _maximum;

	const char *_full_path_video;
	rtos::Thread _video_thread {};
	rtos::EventFlags _event_flags;
};

#define VideoKit_DeclareIRQHandlers(instance)                                                                          \
	extern "C" {                                                                                                       \
	void DSI_IRQHandler(void) { HAL_DSI_IRQHandler(&instance.getDSI().getHandle()); }                                  \
	void JPEG_IRQHandler(void) { HAL_JPEG_IRQHandler(&instance.getJPEG().getHandle()); }                               \
	void DMA2_Stream0_IRQHandler(void) { HAL_DMA_IRQHandler(instance.getJPEG().getHandle().hdmain); }                  \
	void DMA2_Stream1_IRQHandler(void) { HAL_DMA_IRQHandler(instance.getJPEG().getHandle().hdmaout); }                 \
	void DMA2D_IRQHandler(void) { HAL_DMA2D_IRQHandler(&instance.getDMA2D().getHandle()); }                            \
	void LTDC_IRQHandler(void) { HAL_LTDC_IRQHandler(&instance.getLTDC().getHandle()); }                               \
	}

}	// namespace leka
