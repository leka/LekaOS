#include "CoreSTM32Hal.h"

namespace leka {

void CoreSTM32Hal::HAL_RCC_GPIOD_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_GPIOE_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_GPIOF_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOF_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_GPIOG_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOG_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_GPIOH_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOH_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_GPIOI_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOI_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_GPIOJ_CLK_ENABLE(void)
{
	__HAL_RCC_GPIOJ_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_FMC_CLK_ENABLE(void)
{
	__HAL_RCC_FMC_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_DMA2_CLK_ENABLE(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_JPEG_CLK_ENABLE(void)
{
	__HAL_RCC_JPEG_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_LTDC_CLK_ENABLE(void)
{
	__HAL_RCC_LTDC_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_LTDC_FORCE_RESET(void)
{
	__HAL_RCC_LTDC_FORCE_RESET();
}

void CoreSTM32Hal::HAL_RCC_LTDC_RELEASE_RESET(void)
{
	__HAL_RCC_LTDC_RELEASE_RESET();
}

void CoreSTM32Hal::HAL_RCC_DMA2D_CLK_ENABLE(void)
{
	__HAL_RCC_DMA2D_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_DMA2D_FORCE_RESET(void)
{
	__HAL_RCC_DMA2D_FORCE_RESET();
}

void CoreSTM32Hal::HAL_RCC_DMA2D_RELEASE_RESET(void)
{
	__HAL_RCC_DMA2D_RELEASE_RESET();
}

void CoreSTM32Hal::HAL_RCC_DSI_CLK_ENABLE(void)
{
	__HAL_RCC_DSI_CLK_ENABLE();
}

void CoreSTM32Hal::HAL_RCC_DSI_FORCE_RESET(void)
{
	__HAL_RCC_DSI_FORCE_RESET();
}

void CoreSTM32Hal::HAL_RCC_DSI_RELEASE_RESET(void)
{
	__HAL_RCC_DSI_RELEASE_RESET();
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *PeriphClkInit)
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

HAL_StatusTypeDef CoreSTM32Hal::HAL_SDRAM_Init(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_TimingTypeDef *Timing)
{
	return ::HAL_SDRAM_Init(hsdram, Timing);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_SDRAM_SendCommand(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command,
													  uint32_t Timeout)
{
	return ::HAL_SDRAM_SendCommand(hsdram, Command, Timeout);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_SDRAM_ProgramRefreshRate(SDRAM_HandleTypeDef *hsdram, uint32_t RefreshRate)
{
	return ::HAL_SDRAM_ProgramRefreshRate(hsdram, RefreshRate);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{
	return ::HAL_DMA_Init(hdma);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
	return ::HAL_DMA_DeInit(hdma);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DMA2D_Init(DMA2D_HandleTypeDef *hdma2d)
{
	return ::HAL_DMA2D_Init(hdma2d);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DMA2D_ConfigLayer(DMA2D_HandleTypeDef *hdma2d, uint32_t LayerIdx)
{
	return ::HAL_DMA2D_ConfigLayer(hdma2d, LayerIdx);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DMA2D_Start(DMA2D_HandleTypeDef *hdma2d, uint32_t pdata, uint32_t DstAddress,
												uint32_t Width, uint32_t Height)
{
	return ::HAL_DMA2D_Start(hdma2d, pdata, DstAddress, Width, Height);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DMA2D_PollForTransfer(DMA2D_HandleTypeDef *hdma2d, uint32_t Timeout)
{
	return ::HAL_DMA2D_PollForTransfer(hdma2d, Timeout);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DSI_Init(DSI_HandleTypeDef *hdsi, DSI_PLLInitTypeDef *PLLInit)
{
	return ::HAL_DSI_Init(hdsi, PLLInit);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DSI_DeInit(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_DeInit(hdsi);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DSI_ConfigVideoMode(DSI_HandleTypeDef *hdsi, DSI_VidCfgTypeDef *VidCfg)
{
	return ::HAL_DSI_ConfigVideoMode(hdsi, VidCfg);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DSI_Start(DSI_HandleTypeDef *hdsi)
{
	return ::HAL_DSI_Start(hdsi);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DSI_ShortWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode,
												   uint32_t Param1, uint32_t Param2)
{
	return ::HAL_DSI_ShortWrite(hdsi, ChannelID, Mode, Param1, Param2);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_DSI_LongWrite(DSI_HandleTypeDef *hdsi, uint32_t ChannelID, uint32_t Mode,
												  uint32_t NbParams, uint32_t Param1, uint8_t *ParametersTable)
{
	return ::HAL_DSI_LongWrite(hdsi, ChannelID, Mode, NbParams, Param1, ParametersTable);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_LTDC_StructInitFromVideoConfig(LTDC_HandleTypeDef *hltdc, DSI_VidCfgTypeDef *VidCfg)
{
	return ::HAL_LTDC_StructInitFromVideoConfig(hltdc, VidCfg);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_LTDC_Init(LTDC_HandleTypeDef *hltdc)
{
	return ::HAL_LTDC_Init(hltdc);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, LTDC_LayerCfgTypeDef *pLayerCfg,
													 uint32_t LayerIdx)
{
	return ::HAL_LTDC_ConfigLayer(hltdc, pLayerCfg, LayerIdx);
}

HAL_StatusTypeDef CoreSTM32Hal::HAL_JPEG_Init(JPEG_HandleTypeDef *hjpeg)
{
	return ::HAL_JPEG_Init(hjpeg);
}
HAL_StatusTypeDef CoreSTM32Hal::HAL_JPEG_GetInfo(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
	return ::HAL_JPEG_GetInfo(hjpeg, pInfo);
}
HAL_StatusTypeDef CoreSTM32Hal::HAL_JPEG_Decode(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataIn, uint32_t InDataLength,
												uint8_t *pDataOutMCU, uint32_t OutDataLength, uint32_t Timeout)
{
	return ::HAL_JPEG_Decode(hjpeg, pDataIn, InDataLength, pDataOutMCU, OutDataLength, Timeout);
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

}	// namespace leka
