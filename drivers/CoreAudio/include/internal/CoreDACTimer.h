// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_TIMER_H_
#define _LEKA_OS_LIB_DAC_TIMER_H_

#include "LKCoreSTM32HalBase.h"


//TODO : Add final specifiers 

namespace leka {

// Timer class for the DAC
// This class configures the hardware timer TIM6 to output trigger signals to the DAC through the TRGO line and thus without the need of interrupts
class CoreDACTimer
{
  public:
	explicit CoreDACTimer(LKCoreSTM32HalBase &hal);

	void initialize(float frequency);
	void deInitialize();
	void start();
	void stop();
	//void connectToDAC();

	auto getHandle() -> TIM_HandleTypeDef;

  private:
	LKCoreSTM32HalBase &_hal;
	TIM_HandleTypeDef _htim;

	auto calculatePeriod(float freq) -> uint32_t;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_TIMER_H_
