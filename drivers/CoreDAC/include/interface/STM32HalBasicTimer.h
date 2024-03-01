// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/STM32Hal.h"

namespace leka::interface {

class STM32HalBasicTimer
{
  public:
	virtual ~STM32HalBasicTimer() = default;

	[[nodiscard]] virtual auto getHandle() -> TIM_HandleTypeDef & = 0;

	virtual void linkDACTimer(DAC_ChannelConfTypeDef *config) = 0;

	virtual void initialize(uint32_t frequency) = 0;
	virtual void terminate()					= 0;

	virtual void start() = 0;
	virtual void stop()	 = 0;
};

}	// namespace leka::interface
