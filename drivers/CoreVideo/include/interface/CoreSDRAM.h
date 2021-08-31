// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0
// Original file:
// https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_sdram.h

#ifndef _LEKA_OS_LIB_SDRAM_BASE_H_
#define _LEKA_OS_LIB_SDRAM_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka::interface {

class CoreSDRAM
{
  public:
	virtual void setupSDRAMConfig()							= 0;
	virtual FMC_SDRAM_TimingTypeDef setupTimingConfig(void) = 0;
	virtual DMA_HandleTypeDef setupDMA()					= 0;

	virtual uint8_t initialize(void)		  = 0;
	virtual void initializeController()		  = 0;
	virtual void initializationSequence(void) = 0;
};

}	// namespace leka::interface

#endif
