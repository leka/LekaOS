// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_BASE_H_
#define _LEKA_OS_LIB_LTDC_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreLTDCBase
{
  public:
	virtual ~LKCoreLTDCBase() = default;

	virtual void initialize() = 0;

	virtual void setupLayerConfig(void)		 = 0;
	virtual void setupTimingConfig(void)	 = 0;
	virtual void setupBackgroundConfig(void) = 0;

	virtual void configurePeriphClock(void) = 0;
	virtual void configureLayer(void)		= 0;

	virtual LTDC_HandleTypeDef getHandle(void)		  = 0;
	virtual LTDC_LayerCfgTypeDef getLayerConfig(void) = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_BASE_H_
