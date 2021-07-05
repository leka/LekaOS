// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_LTDC_BASE_H_
#define _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_LTDC_BASE_H_

#include "interface/ComponentHandler.h"
#include "stm32f7xx_hal.h"

namespace leka::interface {

class LTDCBase : public ComponentHandler<LTDC_HandleTypeDef>
{
  public:
	virtual ~LTDCBase()		  = default;
	virtual void initialize() = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_LTDC_BASE_H_
