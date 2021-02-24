// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DSI_H_
#define _LEKA_OS_DRIVER_DSI_H_

#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreDSI
{
  public:
	LKCoreDSI(LKCoreSTM32HalBase &hal);

	void initialize();
	void start();
	void reset();

	DSI_HandleTypeDef getHandle();
	DSI_VidCfgTypeDef getConfig();

	void writeCommand(uint8_t *data, uint32_t size);

  private:
	LKCoreSTM32HalBase &_hal;
	DSI_HandleTypeDef _hdsi;
	DSI_VidCfgTypeDef _config;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_DSI_H_
