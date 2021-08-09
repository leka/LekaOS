// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_TIMER_H_
#define _LEKA_OS_LIB_DAC_TIMER_H_

#include <cstdio>

#include "LKCoreSTM32HalBase.h"

namespace leka {

/**
 * @brief  Timer class for the DAC
 * This class configures the hardware timer TIM6 to output trigger signals to the DAC through the TRGO line
 *  and thus without the need of interrupts
 */
class CoreDACTimer
{
  public:
	explicit CoreDACTimer(LKCoreSTM32HalBase &hal);

	void initialize(uint32_t frequency);
	void terminate();
	void start();
	void stop();

	auto getHandle() -> TIM_HandleTypeDef;

  private:
	LKCoreSTM32HalBase &_hal;
	TIM_HandleTypeDef _htim;

	auto _calculatePeriod(uint32_t freq) -> uint32_t;

	void _registerMspCallbacks();
	void _mspInitCallback();
	void _mspDeInitCallback();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_TIMER_H_
