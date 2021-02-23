// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_STM32_HAL_BASE_H_
#define _LEKA_OS_STM32_HAL_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreSTM32HalBase
{
  public:
	virtual ~LKCoreSTM32HalBase() = default;

	virtual HAL_StatusTypeDef HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d)							   = 0;
	virtual HAL_StatusTypeDef HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)	   = 0;
	virtual HAL_StatusTypeDef HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress,
											  uint32_t Width, uint32_t Height)						   = 0;
	virtual HAL_StatusTypeDef HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout) = 0;

  private:
};

}	// namespace leka

#endif	 // _LEKA_OS_STM32_HAL_BASE_H_
