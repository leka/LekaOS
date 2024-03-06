#include "CoreDAC.h"
#include "CoreSTM32HalBasicTimer.h"

extern "C" {

extern leka::CoreSTM32HalBasicTimer hal_timer;
extern leka::CoreDAC coredac;

void TIM6_DAC_IRQHandler()
{
	TIM6->SR = ~TIM_SR_UIF;
	// GPIOG->ODR ^= (1 << 6);
	// HAL_TIM_IRQHandler(&hal_timer.getHandle());
	// have to be replace
	// https://stackoverflow.com/questions/56995426/issue-with-frequency-limitation-on-the-timers-on-stm32f7
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
