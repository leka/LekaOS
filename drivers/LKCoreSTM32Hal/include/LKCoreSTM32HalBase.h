// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_STM32_HAL_BASE_H_
#define _LEKA_OS_STM32_HAL_BASE_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreSTM32HalBase
{
  protected:
	using HALStatus = HAL_StatusTypeDef;

  public:
	virtual ~LKCoreSTM32HalBase() = default;

	virtual void HAL_RCC_GPIOD_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_GPIOE_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_GPIOF_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_GPIOG_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_GPIOH_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_GPIOI_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_GPIOJ_CLK_ENABLE(void) = 0;

	virtual void HAL_RCC_FMC_CLK_ENABLE(void)  = 0;
	virtual void HAL_RCC_DMA2_CLK_ENABLE(void) = 0;
	virtual void HAL_RCC_JPEG_CLK_ENABLE(void) = 0;

	virtual void HAL_RCC_DMA2D_CLK_ENABLE(void)	   = 0;
	virtual void HAL_RCC_DMA2D_FORCE_RESET(void)   = 0;
	virtual void HAL_RCC_DMA2D_RELEASE_RESET(void) = 0;

	virtual void HAL_RCC_LTDC_CLK_ENABLE(void)	  = 0;
	virtual void HAL_RCC_LTDC_FORCE_RESET(void)	  = 0;
	virtual void HAL_RCC_LTDC_RELEASE_RESET(void) = 0;

	virtual void HAL_RCC_DSI_CLK_ENABLE(void)	 = 0;
	virtual void HAL_RCC_DSI_FORCE_RESET(void)	 = 0;
	virtual void HAL_RCC_DSI_RELEASE_RESET(void) = 0;

	virtual HALStatus HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *) = 0;

	virtual void HAL_GPIO_Init(GPIO_TypeDef *, GPIO_InitTypeDef *)			= 0;
	virtual void HAL_GPIO_WritePin(GPIO_TypeDef *, uint16_t, GPIO_PinState) = 0;

	virtual void HAL_NVIC_EnableIRQ(IRQn_Type)						 = 0;
	virtual void HAL_NVIC_SetPriority(IRQn_Type, uint32_t, uint32_t) = 0;

	virtual HALStatus HAL_SDRAM_Init(SDRAM_HandleTypeDef *, FMC_SDRAM_TimingTypeDef *)					 = 0;
	virtual HALStatus HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *, FMC_SDRAM_CommandTypeDef *, uint32_t) = 0;
	virtual HALStatus HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *, uint32_t)						 = 0;

	virtual HALStatus HAL_DMA_Init(DMA_HandleTypeDef *)	  = 0;
	virtual HALStatus HAL_DMA_DeInit(DMA_HandleTypeDef *) = 0;

	virtual HALStatus HAL_DMA2D_Init(DMA2D_HandleTypeDef *)												= 0;
	virtual HALStatus HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *, uint32_t)							= 0;
	virtual HALStatus HAL_DMA2D_Start(DMA2D_HandleTypeDef *, uint32_t, uint32_t, uint32_t, uint32_t)	= 0;
	virtual HALStatus HAL_DMA2D_Start_IT(DMA2D_HandleTypeDef *, uint32_t, uint32_t, uint32_t, uint32_t) = 0;
	virtual HALStatus HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *, uint32_t)						= 0;

	virtual HALStatus HAL_DSI_Init(DSI_HandleTypeDef *, DSI_PLLInitTypeDef *)					 = 0;
	virtual HALStatus HAL_DSI_DeInit(DSI_HandleTypeDef *)										 = 0;
	virtual HALStatus HAL_DSI_Start(DSI_HandleTypeDef *)										 = 0;
	virtual HALStatus HAL_DSI_Refresh(DSI_HandleTypeDef *)										 = 0;
	virtual HALStatus HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *, DSI_VidCfgTypeDef *)			 = 0;
	virtual HALStatus HAL_DSI_ConfigAdaptedCommandMode(DSI_HandleTypeDef *, DSI_CmdCfgTypeDef *) = 0;
	virtual HALStatus HAL_DSI_ConfigCommand(DSI_HandleTypeDef *, DSI_LPCmdTypeDef *)			 = 0;
	virtual HALStatus HAL_DSI_ConfigFlowControl(DSI_HandleTypeDef *, uint32_t)					 = 0;
	virtual HALStatus HAL_DSI_ConfigPhyTimer(DSI_HandleTypeDef *, DSI_PHY_TimerTypeDef *)		 = 0;

	virtual HALStatus HAL_DSI_ShortWrite(DSI_HandleTypeDef *, uint32_t, uint32_t, uint32_t, uint32_t)			= 0;
	virtual HALStatus HAL_DSI_LongWrite(DSI_HandleTypeDef *, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t *) = 0;

	virtual HALStatus HAL_DSI_RegisterCallback(DSI_HandleTypeDef *, HAL_DSI_CallbackIDTypeDef,
											   pDSI_CallbackTypeDef) = 0;

	virtual HALStatus HAL_LTDC_Init(LTDC_HandleTypeDef *)										   = 0;
	virtual HALStatus HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *, LTDC_LayerCfgTypeDef *, uint32_t) = 0;
	virtual HALStatus HAL_LTDC_SetPitch(LTDC_HandleTypeDef *, uint32_t, uint32_t)				   = 0;
	virtual HALStatus HAL_LTDC_SetAddress(LTDC_HandleTypeDef *, uint32_t, uint32_t)				   = 0;

	virtual HALStatus HAL_JPEG_Init(JPEG_HandleTypeDef *)														= 0;
	virtual HALStatus HAL_JPEG_GetInfo(JPEG_HandleTypeDef *, JPEG_ConfTypeDef *)								= 0;
	virtual HALStatus HAL_JPEG_Decode(JPEG_HandleTypeDef *, uint8_t *, uint32_t, uint8_t *, uint32_t, uint32_t) = 0;
	virtual void HAL_JPEG_ConfigInputBuffer(JPEG_HandleTypeDef *, uint8_t *, uint32_t)							= 0;
	virtual void HAL_JPEG_ConfigOutputBuffer(JPEG_HandleTypeDef *, uint8_t *, uint32_t)							= 0;

	virtual HALStatus HAL_JPEG_RegisterInfoReadyCallback(JPEG_HandleTypeDef *, pJPEG_InfoReadyCallbackTypeDef) = 0;
	virtual HALStatus HAL_JPEG_RegisterGetDataCallback(JPEG_HandleTypeDef *, pJPEG_GetDataCallbackTypeDef)	   = 0;
	virtual HALStatus HAL_JPEG_RegisterDataReadyCallback(JPEG_HandleTypeDef *, pJPEG_DataReadyCallbackTypeDef) = 0;
	virtual HALStatus HAL_JPEG_RegisterCallback(JPEG_HandleTypeDef *, HAL_JPEG_CallbackIDTypeDef,
												pJPEG_CallbackTypeDef)										   = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_STM32_HAL_BASE_H_
