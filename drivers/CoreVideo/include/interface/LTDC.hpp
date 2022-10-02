// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/STM32Hal.h"

namespace leka::interface {

class LTDCBase
{
  public:
	virtual ~LTDCBase()		  = default;
	virtual void initialize() = 0;

	virtual auto getHandle() -> LTDC_HandleTypeDef & = 0;
};

}	// namespace leka::interface
