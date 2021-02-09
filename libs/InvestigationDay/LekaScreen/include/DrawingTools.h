// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DRAWING_H_
#define _LEKA_OS_LIB_DRAWING_H_

#include "rtos/ThisThread.h"

#include "LowPowerTimer.h"
#include "dma2d.h"
#include "lcd_properties.h"
#include "ltdc.h"

namespace leka {
namespace draw {
	extern uint32_t default_background_color;

	void toolsInit();
	void clear(uint32_t color = default_background_color);
	void filledRectangle(uint16_t x_0, uint16_t y_0, uint16_t width, uint16_t height, uint32_t color);
}	// namespace draw

namespace screensaver {
	void run(std::chrono::seconds duration_sec = std::chrono::seconds(10));
	void set(void (*pscreen_saver)(std::chrono::seconds duration_sec));
	void squareBouncing(std::chrono::seconds duration_sec);
}	// namespace screensaver
}	// namespace leka

#endif
