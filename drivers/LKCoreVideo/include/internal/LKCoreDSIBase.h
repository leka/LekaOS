// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DSI_BASE_H_
#define _LEKA_OS_DRIVER_DSI_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreDSIBase
{
  public:
	virtual ~LKCoreDSIBase() = default;

	virtual void initialize() = 0;
	virtual void start()	  = 0;
	virtual void reset()	  = 0;

	virtual DSI_VidCfgTypeDef getConfig() = 0;

	virtual void write(const uint8_t *data, const uint32_t size) = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_DSI_BASE_H_
