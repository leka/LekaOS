// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DSI_H_
#define _LEKA_OS_LIB_DSI_H_

#include "lcd_properties.h"
#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreDSI
{
  public:
	LKCoreDSI();

	void initialize();
	void start();
	void reset();

	DSI_VidCfgTypeDef getDsivideoHandler();

	static void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);
	void DSI_IRQHandler(void);
	static void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi);

  private:
	static DSI_HandleTypeDef hdsi;
	DSI_VidCfgTypeDef hdsivideo;
	int DSI_IRQ_counter;

	static int HAL_error_status;
};

}	// namespace leka

void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi);

#endif	 // _LEKA_OS_LIB_DSI_H_
