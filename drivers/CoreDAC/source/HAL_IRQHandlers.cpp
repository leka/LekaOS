#include "CoreDAC.h"
#include "CoreSTM32HalBasicTimer.h"

extern "C" {

namespace audio::internal {
	extern leka::CoreSTM32HalBasicTimer hal_timer;
	extern leka::CoreDAC coredac;
}	// namespace audio::internal

void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&audio::internal::hal_timer.getHandle());
}

void TIM7_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&audio::internal::hal_timer.getHandle());
}

void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(audio::internal::coredac.getHandle().DMA_Handle1);
}

}	// extern "C"
