// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

namespace leka {

CoreDACTimer::CoreDACTimer(LKCoreSTM32HalBase &hal) : _hal(hal)
{
	_htim.Instance = TIM6;
}

void CoreDACTimer::_registerMspCallbacks()
{
	static auto *self = this;
	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPINIT_CB_ID,
								  [](TIM_HandleTypeDef *htim) { self->_mspInitCallback(); });
	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPDEINIT_CB_ID,
								  [](TIM_HandleTypeDef *htim) { self->_mspDeInitCallback(); });
}

void CoreDACTimer::_mspInitCallback()
{
	if (_htim.Instance == TIM6) {
		_hal.HAL_RCC_TIM6_CLK_ENABLE();
	}
}

void CoreDACTimer::_mspDeInitCallback()
{
	if (_htim.Instance == TIM6) {
		_hal.HAL_RCC_TIM6_CLK_DISABLE();
	}
}

}	// namespace leka