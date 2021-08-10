// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_TIMER_H_
#define _LEKA_OS_LIB_DAC_TIMER_H_

#include <cstdio>

#include "LKCoreSTM32HalBase.h"
#include "LogKit.h"

namespace leka {

class CoreDACTimer
{
  public:
	enum class HardWareBasicTimer
	{
		BasicTimer6,
		BasicTimer7
	};

	explicit CoreDACTimer(LKCoreSTM32HalBase &hal, HardWareBasicTimer tim);

	void initialize(uint32_t frequency);
	void terminate();
	void start();
	void stop();

	[[nodiscard]] auto getHandle() const -> const TIM_HandleTypeDef &;
	[[nodiscard]] auto getHardWareBasicTimer() const -> const HardWareBasicTimer &;

  private:
	LKCoreSTM32HalBase &_hal;
	TIM_HandleTypeDef _htim;
	HardWareBasicTimer _hardwareTim;

	auto _calculatePeriod(uint32_t freq) -> uint32_t;

	void _registerMspCallbacks();
	void _msp_onInitializationCb();
	void _msp_onTerminationCb();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_TIMER_H_
