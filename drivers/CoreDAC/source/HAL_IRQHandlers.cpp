#include "CoreSTM32HalBasicTimer.h"

extern "C" {

namespace audio::internal {
	extern leka::CoreSTM32HalBasicTimer hal_timer;
}

void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&audio::internal::hal_timer.getHandle());
}

void TIM7_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&audio::internal::hal_timer.getHandle());
}

}	// extern "C"
