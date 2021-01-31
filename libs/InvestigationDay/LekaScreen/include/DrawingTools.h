// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DRAWING_H_
#define _LEKA_OS_LIB_DRAWING_H_

#include "rtos/ThisThread.h"

#include "Screen.h"
#include "dma2d.h"
#include "ltdc.h"

namespace leka {

void drawingToolsInit(LCD_Model lcd_model);
void drawClear(uint32_t color = 0xffffffff);
void drawFilledRectangle(uint16_t x_0, uint16_t y_0, uint16_t width, uint16_t height, uint32_t color);

void screenSaver();
void setScreenSaver(void (*pscreen_saver)());
void screenSaverSquareBouncing();

}	// namespace leka

#endif
