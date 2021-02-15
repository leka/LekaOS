// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DSI_H_
#define _LEKA_OS_DRIVER_DSI_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreDSI
{
  public:
	LKCoreDSI();

	void initialize();
	void start();
	void reset();

	DSI_VidCfgTypeDef getConfig();

	static void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);

  private:
	static DSI_HandleTypeDef _hdsi;
	DSI_VidCfgTypeDef _config;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_DSI_H_
