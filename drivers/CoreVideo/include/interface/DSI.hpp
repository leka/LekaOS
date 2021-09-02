// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_DSI_BASE_H_
#define _LEKA_OS_DRIVER_DSI_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka::interface {

class CoreDSI
{
  public:
	virtual ~CoreDSI() = default;

	virtual void initialize() = 0;
	virtual void start()	  = 0;
	virtual void reset()	  = 0;

	virtual auto getConfig() -> DSI_VidCfgTypeDef = 0;

	virtual void write(const uint8_t *data, uint32_t size) = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_DRIVER_DSI_BASE_H_
