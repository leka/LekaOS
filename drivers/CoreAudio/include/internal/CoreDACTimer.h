// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_TIMER_H_
#define _LEKA_OS_LIB_DAC_TIMER_H_

#include "LKCoreSTM32HalBase.h"
#include "interface/drivers/DacTimer.h"

namespace leka {

// Timer class for the DAC
// This class configures the hardware timer TIM6 to output trigger signals to the DAC through the TRGO line and thus
// without the need of interrupts
class CoreDACTimer : public interface::DacTimer
{
  public:
	explicit CoreDACTimer(LKCoreSTM32HalBase &hal);

	void initialize(float frequency) final;
	void deInitialize() final;
	void start() final;
	void stop() final;
	// void connectToDAC();

	auto getHandle() -> TIM_HandleTypeDef final;

	void _mspInitCallback() final;	   // Can stay private => Test output after init
	void _mspDeInitCallback() final;   // Can stay private => Test output after init

  protected:
	LKCoreSTM32HalBase &_hal;
	TIM_HandleTypeDef _htim;

	auto _calculatePeriod(float freq) -> uint32_t final;   // Can stay private

	void _registerMspCallbacks() final;	  // Can stay private => Test output after init
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_TIMER_H_
