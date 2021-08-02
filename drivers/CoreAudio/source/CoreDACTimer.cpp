// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

namespace leka {

CoreDACTimer::CoreDACTimer(LKCoreSTM32HalBase &hal) : _hal(hal)
{
	_htim.Instance = TIM6;
}

void CoreDACTimer::initialize(float frequency)
{
	_htim.Init.Prescaler		 = 0;	// no need of prescaler for high frequencies
	_htim.Init.CounterMode		 = TIM_COUNTERMODE_UP;
	_htim.Init.Period			 = _calculatePeriod(frequency);
	_htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	 // no need to change counter period while working

	_registerMspCallbacks();

	_hal.HAL_TIM_Base_Init(&_htim);

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;			   // trigger used for DAC
	sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;   // TODO(): verify utility of this parameter
	_hal.HAL_TIMEx_MasterConfigSynchronization(&_htim, &sMasterConfig);
}

void CoreDACTimer::deInitialize()
{
	_hal.HAL_TIM_Base_DeInit(&_htim);
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