#include "LKCoreSTM32Hal.h"

namespace leka {

LKCoreSTM32Hal::LKCoreSTM32Hal() {}

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

}	// namespace leka
