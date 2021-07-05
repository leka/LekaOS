// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_DSI_BASE_H_
#define _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_DSI_BASE_H_

#include "interface/ComponentHandler.h"
#include "stm32f7xx_hal.h"

namespace leka::interface {

class DSIBase : public ComponentHandler<DSI_HandleTypeDef>
{
  public:
	virtual ~DSIBase() = default;

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

#endif	 // _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_DSI_BASE_H_
