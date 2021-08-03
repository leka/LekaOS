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
	MOCK_METHOD(void, HAL_RCC_GPIOA_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOD_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOE_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOF_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOG_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOH_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOI_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_GPIOJ_CLK_ENABLE, (), (override));

	MOCK_METHOD(void, HAL_RCC_TIM6_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_TIM6_CLK_DISABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_FMC_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_DMA1_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_DMA2_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_JPEG_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_LTDC_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_LTDC_FORCE_RESET, (), (override));
	MOCK_METHOD(void, HAL_RCC_LTDC_RELEASE_RESET, (), (override));
	MOCK_METHOD(void, HAL_RCC_DMA2D_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_DMA2D_FORCE_RESET, (), (override));
	MOCK_METHOD(void, HAL_RCC_DMA2D_RELEASE_RESET, (), (override));
	MOCK_METHOD(void, HAL_RCC_DSI_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_DSI_FORCE_RESET, (), (override));
	MOCK_METHOD(void, HAL_RCC_DSI_RELEASE_RESET, (), (override));
	MOCK_METHOD(void, HAL_RCC_DAC_CLK_ENABLE, (), (override));
	MOCK_METHOD(void, HAL_RCC_DAC_CLK_DISABLE, (), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_RCCEx_PeriphCLKConfig, (RCC_PeriphCLKInitTypeDef * PeriphClkInit), (override));

	MOCK_METHOD(void, HAL_LINKDMA,
				(SDRAM_HandleTypeDef * hsdram, DMA_HandleTypeDef *hdma, DMA_HandleTypeDef dma_handle), (override));

	MOCK_METHOD(void, HAL_GPIO_Init, (GPIO_TypeDef * GPIOx, GPIO_InitTypeDef *GPIO_Init), (override));
	MOCK_METHOD(void, HAL_GPIO_DeInit, (GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin), (override));
	MOCK_METHOD(void, HAL_GPIO_WritePin, (GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState), (override));

	MOCK_METHOD(void, HAL_NVIC_SetPriority, (IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority),
				(override));
	MOCK_METHOD(void, HAL_NVIC_EnableIRQ, (IRQn_Type IRQn), (override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_SDRAM_Init, (SDRAM_HandleTypeDef * hsdram, FMC_SDRAM_TimingTypeDef *Timing),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_SDRAM_SendCommand,
				(SDRAM_HandleTypeDef * hsdram, FMC_SDRAM_CommandTypeDef *Command, uint32_t Timeout), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_SDRAM_ProgramRefreshRate, (SDRAM_HandleTypeDef * hsdram, uint32_t RefreshRate),
				(override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA_Init, (DMA_HandleTypeDef * hdma), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DMA_DeInit, (DMA_HandleTypeDef * hdma), (override));

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

	MOCK_METHOD(HAL_StatusTypeDef, HAL_TIM_Base_Init, (TIM_HandleTypeDef * htim), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_TIMEx_MasterConfigSynchronization,
				(TIM_HandleTypeDef * htim, TIM_MasterConfigTypeDef *sMasterConfig), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_TIM_RegisterCallback,
				(TIM_HandleTypeDef * htim, HAL_TIM_CallbackIDTypeDef CallbackID, pTIM_CallbackTypeDef pCallback),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_TIM_Base_Start, (TIM_HandleTypeDef * htim), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_TIM_Base_Stop, (TIM_HandleTypeDef * htim), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_TIM_Base_DeInit, (TIM_HandleTypeDef * htim), (override));

	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_Init, (DAC_HandleTypeDef * hdac), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_ConfigChannel,
				(DAC_HandleTypeDef * hdac, DAC_ChannelConfTypeDef *sConfig, uint32_t Channel), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_RegisterCallback,
				(DAC_HandleTypeDef * hdac, HAL_DAC_CallbackIDTypeDef CallbackID, pDAC_CallbackTypeDef pCallback),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_Start, (DAC_HandleTypeDef * hdac, uint32_t Channel), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_Start_DMA,
				(DAC_HandleTypeDef * hdac, uint32_t Channel, uint32_t *pData, uint32_t Length, uint32_t Alignment),
				(override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_Stop_DMA, (DAC_HandleTypeDef * hdac, uint32_t Channel), (override));
	MOCK_METHOD(HAL_StatusTypeDef, HAL_DAC_DeInit, (DAC_HandleTypeDef * hdac), (override));
	MOCK_METHOD(uint32_t, HAL_RCC_GetPCLK1Freq, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_STM32_HAL_H_
