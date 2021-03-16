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
	MOCK_METHOD(void, HAL_RCC_GPIOJ_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_JPEG_CLK_ENABLE, (), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_RCCEx_PeriphCLKConfig, (RCC_PeriphCLKInitTypeDef * PeriphClkInit), (override));

	MOCK_METHOD(void, HAL_GPIO_Init, (GPIO_TypeDef * GPIOx, GPIO_InitTypeDef *GPIO_Init), (override));
	MOCK_METHOD(void, HAL_GPIO_WritePin, (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState), (override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_Init, (DMA2D_HandleTypeDef * hdma2d), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_ConfigLayer, (DMA2D_HandleTypeDef * hdma2d, uint32_t LayerIdx),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_Start,
				(DMA2D_HandleTypeDef * hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width, uint32_t Height),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA2D_PollForTransfer, (DMA2D_HandleTypeDef * hdma2d, uint32_t Timeout),
				(override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_DSI_Init, (DSI_HandleTypeDef * hdsi, DSI_PLLInitTypeDef *PLLInit), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DSI_DeInit, (DSI_HandleTypeDef * hdsi), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DSI_ConfigVideoMode, (DSI_HandleTypeDef * hdsi, DSI_VidCfgTypeDef *VidCfg),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DSI_Start, (DSI_HandleTypeDef * hdsi), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DSI_ShortWrite,
				(DSI_HandleTypeDef * hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t Param1, uint32_t Param2),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DSI_LongWrite,
				(DSI_HandleTypeDef * hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t NbParams, uint32_t Param1,
				 uint8_t *ParametersTable),
				(override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_LTDC_StructInitFromVideoConfig,
				(LTDC_HandleTypeDef * hltdc, DSI_VidCfgTypeDef *VidCfg), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_LTDC_Init, (LTDC_HandleTypeDef * hltdc), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_LTDC_ConfigLayer,
				(LTDC_HandleTypeDef * hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx), (override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_JPEG_Init, (JPEG_HandleTypeDef * hjpeg), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_JPEG_GetInfo, (JPEG_HandleTypeDef * hjpeg, JPEG_ConfTypeDef *pInfo), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_JPEG_Decode,
				(JPEG_HandleTypeDef * hjpeg, uint8_t *pDataIn, uint32_t InDataLength, uint8_t *pDataOutMCU,
				 uint32_t OutDataLength, uint32_t Timeout),
				(override));
	MOCK_METHOD(void, HAL_JPEG_ConfigInputBuffer,
				(JPEG_HandleTypeDef * hjpeg, uint8_t *pNewInputBuffer, uint32_t InDataLength), (override));
	MOCK_METHOD(void, HAL_JPEG_ConfigOutputBuffer,
				(JPEG_HandleTypeDef * hjpeg, uint8_t *pNewOutputBuffer, uint32_t OutDataLength), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_STM32_HAL_H_
