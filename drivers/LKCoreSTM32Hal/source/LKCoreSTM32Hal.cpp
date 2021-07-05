#include "LKCoreSTM32Hal.h"

namespace leka {
using LKHal		= LKCoreSTM32Hal;
using HALStatus = HAL_StatusTypeDef;

void LKHal::HAL_RCC_GPIOD_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

void LKHal::HAL_RCC_GPIOE_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
}

void LKHal::HAL_RCC_GPIOF_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOF_CLK_ENABLE();
}

void LKHal::HAL_RCC_GPIOG_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOG_CLK_ENABLE();
}

void LKHal::HAL_RCC_GPIOH_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOH_CLK_ENABLE();
}

void LKHal::HAL_RCC_GPIOI_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOI_CLK_ENABLE();
}

void LKHal::HAL_RCC_GPIOJ_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOJ_CLK_ENABLE();
}

void LKHal::HAL_RCC_FMC_CLK_ENABLE(void)
{
	__HAL_RCC_FMC_CLK_ENABLE();
}

void LKHal::HAL_RCC_DMA2_CLK_ENABLE(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();
}

void LKHal::HAL_RCC_JPEG_CLK_ENABLE(void)
{
	__HAL_RCC_JPEG_CLK_ENABLE();
}

void LKHal::HAL_RCC_LTDC_CLK_ENABLE(void)
{
	__HAL_RCC_LTDC_CLK_ENABLE();
}

void LKHal::HAL_RCC_LTDC_FORCE_RESET(void)
{
	__HAL_RCC_LTDC_FORCE_RESET();
}

void LKHal::HAL_RCC_LTDC_RELEASE_RESET(void)
{
	__HAL_RCC_LTDC_RELEASE_RESET();
}

void LKHal::HAL_RCC_DMA2D_CLK_ENABLE(void)
{
	__HAL_RCC_DMA2D_CLK_ENABLE();
}

void LKHal::HAL_RCC_DMA2D_FORCE_RESET(void)
{
	__HAL_RCC_DMA2D_FORCE_RESET();
}

void LKHal::HAL_RCC_DMA2D_RELEASE_RESET(void)
{
	__HAL_RCC_DMA2D_RELEASE_RESET();
}

void LKHal::HAL_RCC_DSI_CLK_ENABLE(void)
{
	__HAL_RCC_DSI_CLK_ENABLE();
}

void LKHal::HAL_RCC_DSI_FORCE_RESET(void)
{
	__HAL_RCC_DSI_FORCE_RESET();
}

void LKHal::HAL_RCC_DSI_RELEASE_RESET(void)
{
	__HAL_RCC_DSI_RELEASE_RESET();
}

HALStatus LKHal::HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *PeriphClkInit)
{
	return ::HAL_RCCEx_PeriphCLKConfig(PeriphClkInit);
}

void LKHal::HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
	::HAL_GPIO_Init(GPIOx, GPIO_Init);
}

void LKHal::HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	::HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

void LKHal::HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
	::HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);
}

void LKHal::HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
	::HAL_NVIC_EnableIRQ(IRQn);
}

HALStatus LKHal::HAL_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing)
{
	return ::HAL_SDRAM_Init(hsdram, Timing);
}

HALStatus LKHal::HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command, uint32_t Timeout)
{
	return ::HAL_SDRAM_SendCommand(hsdram, Command, Timeout);
}

HALStatus LKHal::HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshRate)
{
	return ::HAL_SDRAM_ProgramRefreshRate(hsdram, RefreshRate);
}

HALStatus LKHal::HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{
	return ::HAL_DMA_Init(hdma);
}

HALStatus LKHal::HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
	return ::HAL_DMA_DeInit(hdma);
}

HALStatus LKHal::HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d)
{
	return ::HAL_DMA2D_Init(hdma2d);
}

HALStatus LKHal::HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)
{
	return ::HAL_DMA2D_ConfigLayer(hdma2d, LayerIdx);
}

HALStatus LKHal::HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t src, uint32_t dst, uint32_t w, uint32_t h)
{
	return ::HAL_DMA2D_Start(hdma2d, src, dst, w, h);
}

HALStatus LKHal::HAL_DMA2D_Start_IT(DMA2D_HandleTypeDef *hdma2d, uint32_t src, uint32_t dst, uint32_t w, uint32_t h)
{
	return ::HAL_DMA2D_Start_IT(hdma2d, src, dst, w, h);
}

HALStatus LKHal::HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout)
{
	return ::HAL_DMA2D_PollForTransfer(hdma2d, Timeout);
}

HALStatus LKHal::HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit)
{
	return ::HAL_DSI_Init(hdsi, PLLInit);
}

HALStatus LKHal::HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_DeInit(hdsi);
}

HALStatus LKHal::HAL_DSI_Start(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_Start(hdsi);
}

HALStatus LKHal::HAL_DSI_Refresh(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_Refresh(hdsi);
}

HALStatus LKHal::HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg)
{
	return ::HAL_DSI_ConfigVideoMode(hdsi, VidCfg);
}

HALStatus LKHal::HAL_DSI_ConfigAdaptedCommandMode(DSI_HandleTypeDef *hdsi, DSI_CmdCfgTypeDef *CmdCfg)
{
	return ::HAL_DSI_ConfigAdaptedCommandMode(hdsi, CmdCfg);
}

HALStatus LKHal::HAL_DSI_ConfigCommand(DSI_HandleTypeDef *hdsi, DSI_LPCmdTypeDef *LPCmd)
{
	return ::HAL_DSI_ConfigCommand(hdsi, LPCmd);
}

HALStatus LKHal::HAL_DSI_ConfigFlowControl(DSI_HandleTypeDef *hdsi, uint32_t FlowControl)
{
	return ::HAL_DSI_ConfigFlowControl(hdsi, FlowControl);
}

HALStatus LKHal::HAL_DSI_ConfigPhyTimer(DSI_HandleTypeDef *hdsi, DSI_PHY_TimerTypeDef *PhyTimers)
{
	return ::HAL_DSI_ConfigPhyTimer(hdsi, PhyTimers);
}

HALStatus LKHal::HAL_DSI_RegisterCallback(DSI_HandleTypeDef *hdsi, HAL_DSI_CallbackIDTypeDef id,
										  pDSI_CallbackTypeDef cb)
{
	return ::HAL_DSI_RegisterCallback(hdsi, id, cb);
}

HALStatus LKHal::HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t Param1,
									uint32_t Param2)
{
	return ::HAL_DSI_ShortWrite(hdsi, ChannelID, Mode, Param1, Param2);
}

HALStatus LKHal::HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode, uint32_t NbParams,
								   uint32_t Param1, uint8_t *ParametersTable)
{
	return ::HAL_DSI_LongWrite(hdsi, ChannelID, Mode, NbParams, Param1, ParametersTable);
}

HALStatus LKHal::HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc)
{
	return ::HAL_LTDC_Init(hltdc);
}

HALStatus LKHal::HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)
{
	return ::HAL_LTDC_ConfigLayer(hltdc, pLayerCfg, LayerIdx);
}

HALStatus LKHal::HAL_LTDC_SetPitch(LTDC_HandleTypeDef *hltdc, uint32_t LinePitch, uint32_t LayerIdx)
{
	return ::HAL_LTDC_SetPitch(hltdc, LinePitch, LayerIdx);
}

HALStatus LKHal::HAL_LTDC_SetAddress(LTDC_HandleTypeDef *hltdc, uint32_t Address, uint32_t LayerIdx)
{
	return ::HAL_LTDC_SetAddress(hltdc, Address, LayerIdx);
}

HALStatus LKHal::HAL_JPEG_Init(JPEG_HandleTypeDef *hjpeg)
{
	return ::HAL_JPEG_Init(hjpeg);
}
HALStatus LKHal::HAL_JPEG_GetInfo(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
	return ::HAL_JPEG_GetInfo(hjpeg, pInfo);
}
HALStatus LKHal::HAL_JPEG_Decode(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
								 uint8_t *pDataOutMCU, uint32_t OutDataLength, uint32_t Timeout)
{
	return ::HAL_JPEG_Decode(hjpeg, pDataIn, InDataLength, pDataOutMCU, OutDataLength, Timeout);
}
void LKHal::HAL_JPEG_ConfigInputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewInputBuffer, uint32_t InDataLength)
{
	::HAL_JPEG_ConfigInputBuffer(hjpeg, pNewInputBuffer, InDataLength);
}
void LKHal::HAL_JPEG_ConfigOutputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewOutputBuffer, uint32_t OutDataLength)
{
	::HAL_JPEG_ConfigOutputBuffer(hjpeg, pNewOutputBuffer, OutDataLength);
}

HALStatus LKHal::HAL_JPEG_RegisterInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_InfoReadyCallbackTypeDef pCallback)
{
	return ::HAL_JPEG_RegisterInfoReadyCallback(hjpeg, pCallback);
}

HALStatus LKHal::HAL_JPEG_RegisterGetDataCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_GetDataCallbackTypeDef pCallback)
{
	return ::HAL_JPEG_RegisterGetDataCallback(hjpeg, pCallback);
}

HALStatus LKHal::HAL_JPEG_RegisterDataReadyCallback(JPEG_HandleTypeDef *hjpeg, pJPEG_DataReadyCallbackTypeDef pCallback)
{
	return ::HAL_JPEG_RegisterDataReadyCallback(hjpeg, pCallback);
}

HALStatus LKHal::HAL_JPEG_RegisterCallback(JPEG_HandleTypeDef *hjpeg, HAL_JPEG_CallbackIDTypeDef id,
										   pJPEG_CallbackTypeDef cb)
{
	return ::HAL_JPEG_RegisterCallback(hjpeg, id, cb);
}

}	// namespace leka
