// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreSTM32Hal.h"

namespace leka {

void CoreSTM32Hal::HAL_RCC_GPIOD_CLK_ENABLE()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_GPIOE_CLK_ENABLE()
{
	__HAL_RCC_GPIOE_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_GPIOF_CLK_ENABLE()
{
	__HAL_RCC_GPIOF_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_GPIOG_CLK_ENABLE()
{
	__HAL_RCC_GPIOG_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_GPIOH_CLK_ENABLE()
{
	__HAL_RCC_GPIOH_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_GPIOI_CLK_ENABLE()
{
	__HAL_RCC_GPIOI_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_GPIOJ_CLK_ENABLE()
{
	__HAL_RCC_GPIOJ_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_TIM6_CLK_ENABLE()
{
	__HAL_RCC_TIM6_CLK_ENABLE();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_TIM6_CLK_DISABLE()
{
	__HAL_RCC_TIM6_CLK_DISABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_TIM7_CLK_ENABLE()
{
	__HAL_RCC_TIM7_CLK_ENABLE();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_TIM7_CLK_DISABLE()
{
	__HAL_RCC_TIM7_CLK_DISABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_FMC_CLK_ENABLE()
{
	__HAL_RCC_FMC_CLK_ENABLE();	  // NOLINT
}

void CoreSTM32Hal::HAL_RCC_DMA2_CLK_ENABLE()
{
	__HAL_RCC_DMA2_CLK_ENABLE();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_JPEG_CLK_ENABLE()
{
	__HAL_RCC_JPEG_CLK_ENABLE();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_JPEG_FORCE_RESET()
{
	__HAL_RCC_JPEG_FORCE_RESET();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_JPEG_RELEASE_RESET()
{
	__HAL_RCC_JPEG_RELEASE_RESET();	  // NOLINT
}

void CoreSTM32Hal::HAL_RCC_LTDC_CLK_ENABLE()
{
	__HAL_RCC_LTDC_CLK_ENABLE();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_LTDC_FORCE_RESET()
{
	__HAL_RCC_LTDC_FORCE_RESET();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_LTDC_RELEASE_RESET()
{
	__HAL_RCC_LTDC_RELEASE_RESET();	  // NOLINT
}

void CoreSTM32Hal::HAL_RCC_DMA2D_CLK_ENABLE()
{
	__HAL_RCC_DMA2D_CLK_ENABLE();	// NOLINT
}

void CoreSTM32Hal::HAL_RCC_DMA2D_FORCE_RESET()
{
	__HAL_RCC_DMA2D_FORCE_RESET();	 // NOLINT
}

void CoreSTM32Hal::HAL_RCC_DMA2D_RELEASE_RESET()
{
	__HAL_RCC_DMA2D_RELEASE_RESET();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_DSI_CLK_ENABLE()
{
	__HAL_RCC_DSI_CLK_ENABLE();	  // NOLINT
}

void CoreSTM32Hal::HAL_RCC_DSI_FORCE_RESET()
{
	__HAL_RCC_DSI_FORCE_RESET();   // NOLINT
}

void CoreSTM32Hal::HAL_RCC_DSI_RELEASE_RESET()
{
	__HAL_RCC_DSI_RELEASE_RESET();	 // NOLINT
}

auto CoreSTM32Hal::HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *PeriphClkInit) -> HAL_StatusTypeDef
{
	return ::HAL_RCCEx_PeriphCLKConfig(PeriphClkInit);
}

void CoreSTM32Hal::HAL_LINKDMA(SDRAM_HandleTypeDef *hsdram, DMA_HandleTypeDef *hdma, DMA_HandleTypeDef dma_handle)
{
	__HAL_LINKDMA(hsdram, hdma, dma_handle);
}

void CoreSTM32Hal::HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
	::HAL_GPIO_Init(GPIOx, GPIO_Init);
}

void CoreSTM32Hal::HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	::HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

void CoreSTM32Hal::HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
	::HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);
}

void CoreSTM32Hal::HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
	::HAL_NVIC_EnableIRQ(IRQn);
}

auto CoreSTM32Hal::HAL_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing) -> HAL_StatusTypeDef
{
	return ::HAL_SDRAM_Init(hsdram, Timing);
}

auto CoreSTM32Hal::HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command,
										 uint32_t Timeout) -> HAL_StatusTypeDef
{
	return ::HAL_SDRAM_SendCommand(hsdram, Command, Timeout);
}

auto CoreSTM32Hal::HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshRate) -> HAL_StatusTypeDef
{
	return ::HAL_SDRAM_ProgramRefreshRate(hsdram, RefreshRate);
}

auto CoreSTM32Hal::HAL_DMA_Init(DMA_HandleTypeDef *hdma) -> HAL_StatusTypeDef
{
	return ::HAL_DMA_Init(hdma);
}

auto CoreSTM32Hal::HAL_DMA_DeInit(DMA_HandleTypeDef *hdma) -> HAL_StatusTypeDef
{
	return ::HAL_DMA_DeInit(hdma);
}

auto CoreSTM32Hal::HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d) -> HAL_StatusTypeDef
{
	return ::HAL_DMA2D_Init(hdma2d);
}

auto CoreSTM32Hal::HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx) -> HAL_StatusTypeDef
{
	return ::HAL_DMA2D_ConfigLayer(hdma2d, LayerIdx);
}

auto CoreSTM32Hal::HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,
								   uint32_t Height) -> HAL_StatusTypeDef
{
	return ::HAL_DMA2D_Start(hdma2d, pdata, DstAddress, Width, Height);
}

auto CoreSTM32Hal::HAL_DMA2D_Start_IT(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress, uint32_t Width,
									  uint32_t Height) -> HAL_StatusTypeDef
{
	return ::HAL_DMA2D_Start_IT(hdma2d, pdata, DstAddress, Width, Height);
}

auto CoreSTM32Hal::HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout) -> HAL_StatusTypeDef
{
	return ::HAL_DMA2D_PollForTransfer(hdma2d, Timeout);
}

auto CoreSTM32Hal::HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit) -> HAL_StatusTypeDef
{
	return ::HAL_DSI_Init(hdsi, PLLInit);
}

auto CoreSTM32Hal::HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi) -> HAL_StatusTypeDef
{
	return ::HAL_DSI_DeInit(hdsi);
}

auto CoreSTM32Hal::HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg) -> HAL_StatusTypeDef
{
	return ::HAL_DSI_ConfigVideoMode(hdsi, VidCfg);
}

auto CoreSTM32Hal::HAL_DSI_Start(DSI_HandleTypeDef *hdsi) -> HAL_StatusTypeDef
{
	return ::HAL_DSI_Start(hdsi);
}

auto CoreSTM32Hal::HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t Param1,
									  uint32_t Param2) -> HAL_StatusTypeDef
{
	return ::HAL_DSI_ShortWrite(hdsi, ChannelID, Mode, Param1, Param2);
}

auto CoreSTM32Hal::HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t NbParams,
									 uint32_t Param1, uint8_t *ParametersTable) -> HAL_StatusTypeDef
{
	return ::HAL_DSI_LongWrite(hdsi, ChannelID, Mode, NbParams, Param1, ParametersTable);
}

auto CoreSTM32Hal::HAL_LTDC_StructInitFromVideoConfig(LTDC_HandleTypeDef *hltdc, DSI_VidCfgTypeDef *VidCfg)
	-> HAL_StatusTypeDef
{
	return ::HAL_LTDC_StructInitFromVideoConfig(hltdc, VidCfg);
}

auto CoreSTM32Hal::HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc) -> HAL_StatusTypeDef
{
	return ::HAL_LTDC_Init(hltdc);
}

auto CoreSTM32Hal::HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)
	-> HAL_StatusTypeDef
{
	return ::HAL_LTDC_ConfigLayer(hltdc, pLayerCfg, LayerIdx);
}

auto CoreSTM32Hal::HAL_JPEG_Init(JPEG_HandleTypeDef *hjpeg) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_Init(hjpeg);
}

auto CoreSTM32Hal::HAL_JPEG_GetInfo(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_GetInfo(hjpeg, pInfo);
}

auto CoreSTM32Hal::HAL_JPEG_Decode(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
								   uint8_t *pDataOutMCU, uint32_t OutDataLength, uint32_t Timeout) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_Decode(hjpeg, pDataIn, InDataLength, pDataOutMCU, OutDataLength, Timeout);
}

auto CoreSTM32Hal::HAL_JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
									   uint8_t *pDataOutMCU, uint32_t OutDataLength) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_Decode_DMA(hjpeg, pDataIn, InDataLength, pDataOutMCU, OutDataLength);
}

void CoreSTM32Hal::HAL_JPEG_ConfigInputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewInputBuffer,
											  uint32_t InDataLength)
{
	::HAL_JPEG_ConfigInputBuffer(hjpeg, pNewInputBuffer, InDataLength);
}

void CoreSTM32Hal::HAL_JPEG_ConfigOutputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewOutputBuffer,
											   uint32_t OutDataLength)
{
	::HAL_JPEG_ConfigOutputBuffer(hjpeg, pNewOutputBuffer, OutDataLength);
}

auto CoreSTM32Hal::HAL_JPEG_RegisterInfoReadyCallback(JPEG_HandleTypeDef *hjpeg,
													  pJPEG_InfoReadyCallbackTypeDef pCallback) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_RegisterInfoReadyCallback(hjpeg, pCallback);
}

auto CoreSTM32Hal::HAL_JPEG_RegisterGetDataCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_GetDataCallbackTypeDef pCallback)
	-> HAL_StatusTypeDef
{
	return ::HAL_JPEG_RegisterGetDataCallback(hjpeg, pCallback);
}

auto CoreSTM32Hal::HAL_JPEG_RegisterDataReadyCallback(JPEG_HandleTypeDef *hjpeg,
													  pJPEG_DataReadyCallbackTypeDef pCallback) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_RegisterDataReadyCallback(hjpeg, pCallback);
}

auto CoreSTM32Hal::HAL_JPEG_RegisterCallback(JPEG_HandleTypeDef *hjpeg, HAL_JPEG_CallbackIDTypeDef callbackID,
											 pJPEG_CallbackTypeDef pCallback) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_RegisterCallback(hjpeg, callbackID, pCallback);
}

auto CoreSTM32Hal::HAL_JPEG_Pause(JPEG_HandleTypeDef *hjpeg, uint32_t XferSelection) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_Pause(hjpeg, XferSelection);
}

auto CoreSTM32Hal::HAL_JPEG_Resume(JPEG_HandleTypeDef *hjpeg, uint32_t XferSelection) -> HAL_StatusTypeDef
{
	return ::HAL_JPEG_Resume(hjpeg, XferSelection);
}

auto CoreSTM32Hal::HAL_TIM_Base_Init(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef
{
	return ::HAL_TIM_Base_Init(htim);
}

auto CoreSTM32Hal::HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim,
														 TIM_MasterConfigTypeDef *sMasterConfig) -> HAL_StatusTypeDef
{
	return ::HAL_TIMEx_MasterConfigSynchronization(htim, sMasterConfig);
}

auto CoreSTM32Hal::HAL_TIM_RegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID,
											pTIM_CallbackTypeDef pCallback) -> HAL_StatusTypeDef
{
	return ::HAL_TIM_RegisterCallback(htim, CallbackID, pCallback);
}

auto CoreSTM32Hal::HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef
{
	return ::HAL_TIM_Base_Start_IT(htim);
}

auto CoreSTM32Hal::HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef
{
	return ::HAL_TIM_Base_Stop_IT(htim);
}

auto CoreSTM32Hal::HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim) -> HAL_StatusTypeDef
{
	return ::HAL_TIM_Base_DeInit(htim);
}

}	// namespace leka
