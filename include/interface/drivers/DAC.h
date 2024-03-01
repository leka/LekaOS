// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/STM32Hal.h"

namespace leka::interface {

class DACBase
{
  public:
	virtual ~DACBase() = default;

	[[nodiscard]] virtual auto getHandle() -> DAC_HandleTypeDef & = 0;

	virtual void initialize() = 0;
	virtual void terminate()  = 0;

	virtual void start() = 0;
	virtual void stop()	 = 0;
};

}	// namespace leka::interface
