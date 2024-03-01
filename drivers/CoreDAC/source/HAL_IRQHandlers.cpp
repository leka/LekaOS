#include "CoreDAC.h"
#include "CoreSTM32HalBasicTimer.h"

extern "C" {

extern leka::CoreSTM32HalBasicTimer hal_timer;
extern leka::CoreDAC coredac;

void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&hal_timer.getHandle());
}

void TIM7_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&hal_timer.getHandle());
}

void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coredac.getHandle().DMA_Handle1);
}

}	// extern "C"
