// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_INTERFACE_DSI_H_
#define _LEKA_OS_DRIVER_INTERFACE_DSI_H_

#include "ComponentHandler.h"
#include "stm32f7xx_hal.h"

namespace leka::interface {

class CoreDSI : public ComponentHandler<DSI_HandleTypeDef>
{
  public:
	virtual ~CoreDSI() = default;

	virtual void initialize() = 0;
	virtual void reset()	  = 0;
	virtual void refresh()	  = 0;

	virtual void enableLPCmd()	= 0;
	virtual void disableLPCmd() = 0;

	virtual void enableTearingEffectReporting() = 0;

	virtual auto refreshDone() -> bool = 0;

	virtual void write(const uint8_t *data, uint32_t size) = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_DRIVER_INTERFACE_DSI_H_
