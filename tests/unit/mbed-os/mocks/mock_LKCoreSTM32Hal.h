// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_STM32_HAL_MOCK_H_
#define _LEKA_OS_STM32_HAL_MOCK_H_

#include "LKCoreSTM32HalBase.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreSTM32HalMock : public LKCoreSTM32HalBase
{
  public:
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_Init, (DMA2D_HandleTypeDef * hdma2d), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_ConfigLayer, (DMA2D_HandleTypeDef * hdma2d, uint32_t LayerIdx),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_Start,
				(DMA2D_HandleTypeDef * hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width, uint32_t Height),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_PollForTransfer, (DMA2D_HandleTypeDef * hdma2d, uint32_t Timeout),
				(override));
};

}	// namespace leka

#endif	 // _LEKA_OS_STM32_HAL_H_
