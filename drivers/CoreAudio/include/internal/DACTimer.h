// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_TIMER_H_
#define _LEKA_OS_LIB_DAC_TIMER_H_

#include <cstdio>

#include "LKCoreSTM32HalBase.h"
#include "LogKit.h"

namespace leka {

class DACTimer
{
  public:
	enum class HardwareBasicTimer
	{
		BasicTimer6,
		BasicTimer7
	};

	explicit DACTimer(LKCoreSTM32HalBase &hal, HardwareBasicTimer tim);

	void initialize(uint32_t frequency);
	void terminate();
	void start();
	void stop();

	[[nodiscard]] auto getHandle() const -> const TIM_HandleTypeDef &;
	[[nodiscard]] auto getHardWareBasicTimer() const -> const HardwareBasicTimer &;

  private:
	LKCoreSTM32HalBase &_hal;
	TIM_HandleTypeDef _htim;
	HardwareBasicTimer _hardwareTim;

	auto _calculatePeriod(uint32_t freq) -> uint32_t;

	void _registerMspCallbacks();
	void _msp_onInitializationCb();
	void _msp_onTerminationCb();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_TIMER_H_
