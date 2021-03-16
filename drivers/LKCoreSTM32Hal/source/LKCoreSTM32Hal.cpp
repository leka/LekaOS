#include "LKCoreSTM32Hal.h"

namespace leka {

LKCoreSTM32Hal::LKCoreSTM32Hal() {}

void LKCoreSTM32Hal::HAL_RCC_GPIOJ_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOJ_CLK_ENABLE();
}

void LKCoreSTM32Hal::HAL_RCC_JPEG_CLK_ENABLE(void)
{
	__HAL_RCC_JPEG_CLK_ENABLE();
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *PeriphClkInit)
{
	return ::HAL_RCCEx_PeriphCLKConfig(PeriphClkInit);
}

void LKCoreSTM32Hal::HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
	::HAL_GPIO_Init(GPIOx, GPIO_Init);
}

void LKCoreSTM32Hal::HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	::HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d)
{
	return ::HAL_DMA2D_Init(hdma2d);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)
{
	return ::HAL_DMA2D_ConfigLayer(hdma2d, LayerIdx);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress,
												  uint32_t Width, uint32_t Height)
{
	return ::HAL_DMA2D_Start(hdma2d, pdata, DstAddress, Width, Height);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout)
{
	return ::HAL_DMA2D_PollForTransfer(hdma2d, Timeout);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit)
{
	return ::HAL_DSI_Init(hdsi, PLLInit);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_DeInit(hdsi);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg)
{
	return ::HAL_DSI_ConfigVideoMode(hdsi, VidCfg);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DSI_Start(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_Start(hdsi);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode,
													 uint32_t Param1, uint32_t Param2)
{
	return ::HAL_DSI_ShortWrite(hdsi, ChannelID, Mode, Param1, Param2);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode,
													uint32_t NbParams, uint32_t Param1, uint8_t *ParametersTable)
{
	return ::HAL_DSI_LongWrite(hdsi, ChannelID, Mode, NbParams, Param1, ParametersTable);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_LTDC_StructInitFromVideoConfig(LTDC_HandleTypeDef *hltdc,
																	 DSI_VidCfgTypeDef *VidCfg)
{
	return ::HAL_LTDC_StructInitFromVideoConfig(hltdc, VidCfg);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc)
{
	return ::HAL_LTDC_Init(hltdc);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg,
													   uint32_t LayerIdx)
{
	return ::HAL_LTDC_ConfigLayer(hltdc, pLayerCfg, LayerIdx);
}

HAL_StatusTypeDef LKCoreSTM32Hal::HAL_JPEG_Init(JPEG_HandleTypeDef *hjpeg)
{
	return ::HAL_JPEG_Init(hjpeg);
}
HAL_StatusTypeDef LKCoreSTM32Hal::HAL_JPEG_GetInfo(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
	return ::HAL_JPEG_GetInfo(hjpeg, pInfo);
}
HAL_StatusTypeDef LKCoreSTM32Hal::HAL_JPEG_Decode(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
												  uint8_t *pDataOutMCU, uint32_t OutDataLength, uint32_t Timeout)
{
	return ::HAL_JPEG_Decode(hjpeg, pDataIn, InDataLength, pDataOutMCU, OutDataLength, Timeout);
}
void LKCoreSTM32Hal::HAL_JPEG_ConfigInputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewInputBuffer,
												uint32_t InDataLength)
{
	::HAL_JPEG_ConfigInputBuffer(hjpeg, pNewInputBuffer, InDataLength);
}
void LKCoreSTM32Hal::HAL_JPEG_ConfigOutputBuffer(JPEG_HandleTypeDef *hjpeg, uint8_t *pNewOutputBuffer,
												 uint32_t OutDataLength)
{
	::HAL_JPEG_ConfigOutputBuffer(hjpeg, pNewOutputBuffer, OutDataLength);
}

}	// namespace leka
