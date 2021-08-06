// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_BASIC_TIMER_H_
#define _LEKA_OS_INTERFACE_DRIVER_BASIC_TIMER_H_

#include "stm32f7xx_hal.h"

namespace leka::interface::stm32 {

class BasicTimer
{
  public:
	~BasicTimer() = default;

	virtual void initialize(uint32_t frequency) = 0;
	virtual void terminate()					= 0;
	virtual void start()						= 0;
	virtual void stop()							= 0;

	virtual auto getHandle() -> TIM_HandleTypeDef = 0;

  private:
	virtual auto _calculatePeriod(uint32_t freq) -> uint32_t = 0;

	virtual void _registerMspCallbacks() = 0;
	virtual void _mspInitCallback()		 = 0;
	virtual void _mspDeInitCallback()	 = 0;
};

}	// namespace leka::interface::stm32

#endif	 // _LEKA_OS_INTERFACE_DRIVER_BASIC_TIMER_H_
