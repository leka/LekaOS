// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "stm32f7xx_hal.h"

namespace leka::interface {

class STM32Hal
{
  public:
	virtual ~STM32Hal() = default;

	virtual void HAL_RCC_GPIOD_CLK_ENABLE() = 0;
	virtual void HAL_RCC_GPIOE_CLK_ENABLE() = 0;
	virtual void HAL_RCC_GPIOF_CLK_ENABLE() = 0;
	virtual void HAL_RCC_GPIOG_CLK_ENABLE() = 0;
	virtual void HAL_RCC_GPIOH_CLK_ENABLE() = 0;
	virtual void HAL_RCC_GPIOI_CLK_ENABLE() = 0;
	virtual void HAL_RCC_GPIOJ_CLK_ENABLE() = 0;

	virtual void HAL_RCC_TIM6_CLK_ENABLE()	= 0;
	virtual void HAL_RCC_TIM6_CLK_DISABLE() = 0;

	virtual void HAL_RCC_TIM7_CLK_ENABLE()	= 0;
	virtual void HAL_RCC_TIM7_CLK_DISABLE() = 0;

	virtual void HAL_RCC_FMC_CLK_ENABLE() = 0;

	virtual void HAL_RCC_DMA1_CLK_ENABLE() = 0;
	virtual void HAL_RCC_DMA2_CLK_ENABLE() = 0;

	virtual void HAL_RCC_DAC_CLK_ENABLE()  = 0;
	virtual void HAL_RCC_DAC_CLK_DISABLE() = 0;

	virtual void HAL_RCC_JPEG_CLK_ENABLE()	  = 0;
	virtual void HAL_RCC_JPEG_FORCE_RESET()	  = 0;
	virtual void HAL_RCC_JPEG_RELEASE_RESET() = 0;

	virtual void HAL_RCC_LTDC_CLK_ENABLE()	  = 0;
	virtual void HAL_RCC_LTDC_FORCE_RESET()	  = 0;
	virtual void HAL_RCC_LTDC_RELEASE_RESET() = 0;

	virtual void HAL_RCC_DMA2D_CLK_ENABLE()	   = 0;
	virtual void HAL_RCC_DMA2D_FORCE_RESET()   = 0;
	virtual void HAL_RCC_DMA2D_RELEASE_RESET() = 0;

	virtual void HAL_RCC_DSI_CLK_ENABLE()	 = 0;
	virtual void HAL_RCC_DSI_FORCE_RESET()	 = 0;
	virtual void HAL_RCC_DSI_RELEASE_RESET() = 0;

	virtual auto HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *PeriphClkInit) -> HAL_StatusTypeDef = 0;

	virtual void HAL_LINKDMA(SDRAM_HandleTypeDef *hsdram, DMA_HandleTypeDef *hdma, DMA_HandleTypeDef dma_handle) = 0;

	virtual void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)				   = 0;
	virtual void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) = 0;

	virtual void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority) = 0;
	virtual void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)													  = 0;

	virtual auto HAL_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command, uint32_t Timeout)
		-> HAL_StatusTypeDef = 0;
	virtual auto HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshRate)
		-> HAL_StatusTypeDef = 0;

	virtual auto HAL_DMA_Init(DMA_HandleTypeDef *hdma) -> HAL_StatusTypeDef	  = 0;
	virtual auto HAL_DMA_DeInit(DMA_HandleTypeDef *hdma) -> HAL_StatusTypeDef = 0;

	virtual auto HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d) -> HAL_StatusTypeDef							= 0;
	virtual auto HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,
								 uint32_t Height) -> HAL_StatusTypeDef										= 0;
	virtual auto HAL_DMA2D_Start_IT(DMA2D_HandleTypeDef *hdma2d, uint32_t, uint32_t, uint32_t, uint32_t)
		-> HAL_StatusTypeDef																				   = 0;
	virtual auto HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout) -> HAL_StatusTypeDef = 0;

	virtual auto HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit) -> HAL_StatusTypeDef		  = 0;
	virtual auto HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi) -> HAL_StatusTypeDef									  = 0;
	virtual auto HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_DSI_Start(DSI_HandleTypeDef *hdsi) -> HAL_StatusTypeDef									  = 0;
	virtual auto HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t Param1,
									uint32_t Param2) -> HAL_StatusTypeDef										  = 0;
	virtual auto HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t NbParams,
								   uint32_t Param1, uint8_t *ParametersTable) -> HAL_StatusTypeDef				  = 0;

	virtual auto HAL_LTDC_StructInitFromVideoConfig(LTDC_HandleTypeDef *hltdc, DSI_VidCfgTypeDef *VidCfg)
		-> HAL_StatusTypeDef												   = 0;
	virtual auto HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)
		-> HAL_StatusTypeDef = 0;

	virtual auto HAL_JPEG_Init(JPEG_HandleTypeDef *hjpeg) -> HAL_StatusTypeDef							   = 0;
	virtual auto HAL_JPEG_GetInfo(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_JPEG_Decode(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
								 uint8_t *pDataOutMCU, uint32_t OutDataLength, uint32_t Timeout)
		-> HAL_StatusTypeDef																			= 0;
	virtual auto HAL_JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
									 uint8_t *pDataOutMCU, uint32_t OutDataLength) -> HAL_StatusTypeDef = 0;
	virtual void HAL_JPEG_ConfigInputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewInputBuffer,
											uint32_t InDataLength)										= 0;
	virtual void HAL_JPEG_ConfigOutputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewOutputBuffer,
											 uint32_t OutDataLength)									= 0;

	virtual auto HAL_JPEG_RegisterInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_InfoReadyCallbackTypeDef)
		-> HAL_StatusTypeDef = 0;
	virtual auto HAL_JPEG_RegisterGetDataCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_GetDataCallbackTypeDef)
		-> HAL_StatusTypeDef = 0;
	virtual auto HAL_JPEG_RegisterDataReadyCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_DataReadyCallbackTypeDef)
		-> HAL_StatusTypeDef = 0;
	virtual auto HAL_JPEG_RegisterCallback(JPEG_HandleTypeDef *hjpeg, HAL_JPEG_CallbackIDTypeDef, pJPEG_CallbackTypeDef)
		-> HAL_StatusTypeDef = 0;

	virtual auto HAL_JPEG_Pause(JPEG_HandleTypeDef *hjpeg, uint32_t XferSelection) -> HAL_StatusTypeDef	 = 0;
	virtual auto HAL_JPEG_Resume(JPEG_HandleTypeDef *hjpeg, uint32_t XferSelection) -> HAL_StatusTypeDef = 0;

	virtual auto HAL_TIM_Base_Init(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim, TIM_MasterConfigTypeDef *sMasterConfig)
		-> HAL_StatusTypeDef																   = 0;
	virtual auto HAL_TIM_RegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID,
										  pTIM_CallbackTypeDef pCallback) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef		   = 0;
	virtual auto HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef			   = 0;
	virtual auto HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef			   = 0;

	virtual auto HAL_DAC_Init(DAC_HandleTypeDef *hdac) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_DAC_ConfigChannel(DAC_HandleTypeDef *hdac, DAC_ChannelConfTypeDef *sConfig, uint32_t Channel)
		-> HAL_StatusTypeDef																	  = 0;
	virtual auto HAL_DAC_RegisterCallback(DAC_HandleTypeDef *hdac, HAL_DAC_CallbackIDTypeDef CallbackID,
										  pDAC_CallbackTypeDef pCallback) -> HAL_StatusTypeDef	  = 0;
	virtual auto HAL_DAC_Start_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t *pData, uint32_t Length,
								   uint32_t Alignment) -> HAL_StatusTypeDef						  = 0;
	virtual auto HAL_DAC_Stop_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel) -> HAL_StatusTypeDef = 0;
	virtual auto HAL_DAC_DeInit(DAC_HandleTypeDef *hdac) -> HAL_StatusTypeDef					  = 0;
};

}	// namespace leka::interface
