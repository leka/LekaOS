// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_BASE_H_
#define _LEKA_OS_LIB_LTDC_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka::interface {

class LKCoreLTDC
{
  public:
	virtual ~LKCoreLTDC() = default;
	virtual void initialize() = 0;

	virtual auto getHandle() -> LTDC_HandleTypeDef & = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_BASE_H_
