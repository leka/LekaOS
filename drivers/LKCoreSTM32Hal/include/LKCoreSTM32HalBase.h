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

	virtual void HAL_RCC_GPIOJ_CLK_ENABLE(void) = 0;

	virtual void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)				   = 0;
	virtual void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) = 0;

	virtual HAL_StatusTypeDef HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d)							   = 0;
	virtual HAL_StatusTypeDef HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)	   = 0;
	virtual HAL_StatusTypeDef HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress,
											  uint32_t Width, uint32_t Height)						   = 0;
	virtual HAL_StatusTypeDef HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout) = 0;

	virtual HAL_StatusTypeDef HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit)			  = 0;
	virtual HAL_StatusTypeDef HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi)										  = 0;
	virtual HAL_StatusTypeDef HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg)	  = 0;
	virtual HAL_StatusTypeDef HAL_DSI_Start(DSI_HandleTypeDef *hdsi)										  = 0;
	virtual HAL_StatusTypeDef HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode,
												 uint32_t Param1, uint32_t Param2)							  = 0;
	virtual HAL_StatusTypeDef HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode,
												uint32_t NbParams, uint32_t Param1, uint8_t *ParametersTable) = 0;

  private:
};

}	// namespace leka

#endif	 // _LEKA_OS_STM32_HAL_BASE_H_
