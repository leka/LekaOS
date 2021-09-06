// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// * Original file:
// * https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_sdram.h

#ifndef _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_SDRAM_H_
#define _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_SDRAM_H_

#include "stm32f7xx_hal.h"

namespace leka::interface {

class SDRAM
{
  public:
	virtual void setupSDRAMConfig()								= 0;
	virtual auto setupTimingConfig() -> FMC_SDRAM_TimingTypeDef = 0;
	virtual auto setupDMA() -> DMA_HandleTypeDef				= 0;

	virtual auto initialize() -> uint8_t  = 0;
	virtual void initializeController()	  = 0;
	virtual void initializationSequence() = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_DRIVER_CORE_VIDEO_INTERFACE_SDRAM_H_
