// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DSI_H_
#define _LEKA_OS_LIB_DSI_H_

#include "Screen.h"
#include "stm32f7xx_hal.h"

namespace leka {
extern DSI_HandleTypeDef hdsi;
extern DSI_VidCfgTypeDef hdsivideo;
extern int DSI_IRQ_counter;

namespace dsi {
	void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);

	void DSI_IRQHandler(void);
	void init(LCD_Model lcd_model);
	void start();
	void reset();
}	// namespace dsi
}	// namespace leka

void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi);

#endif	 // _LEKA_OS_LIB_DSI_H_
