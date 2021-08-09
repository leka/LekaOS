// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

namespace leka {

CoreDACTimer::CoreDACTimer(LKCoreSTM32HalBase &hal) : _hal(hal)
{
	_htim.Instance = TIM6;
}

void CoreDACTimer::initialize(uint32_t frequency)
{
	_htim.Init.Prescaler		 = 0;	// no need of prescaler for high frequencies
	_htim.Init.CounterMode		 = TIM_COUNTERMODE_UP;
	_htim.Init.Period			 = _calculatePeriod(frequency);
	_htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;	 // no need to change counter period while working

	_registerMspCallbacks();

	_hal.HAL_TIM_Base_Init(&_htim);

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;   // trigger used for DAC
	sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;
	_hal.HAL_TIMEx_MasterConfigSynchronization(&_htim, &sMasterConfig);
}

void CoreDACTimer::terminate()
{
	_hal.HAL_TIM_Base_DeInit(&_htim);
}

void CoreDACTimer::start()
{
	_hal.HAL_TIM_Base_Start(&_htim);
}

void CoreDACTimer::stop()
{
	_hal.HAL_TIM_Base_Stop(&_htim);
}

auto CoreDACTimer::getHandle() -> TIM_HandleTypeDef
{
	return this->_htim;
}

auto CoreDACTimer::_calculatePeriod(uint32_t frequency) -> uint32_t
{
	uint32_t clockFreq = _hal.HAL_RCC_GetPCLK1Freq();

	/* Get PCLK1 prescaler */
	if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0) {
		clockFreq *= 2;
	}

	if (frequency < (clockFreq >> 16) || frequency > clockFreq) {
		printf("Chosen freq out of bounds\n");
	}

	return (clockFreq / frequency);
}

void CoreDACTimer::_registerMspCallbacks()
{
	static auto *self	= this;
	auto initCbLambda	= []([[maybe_unused]] TIM_HandleTypeDef *htim) { self->_mspInitCallback(); };
	auto deInitCbLambda = []([[maybe_unused]] TIM_HandleTypeDef *htim) { self->_mspDeInitCallback(); };
	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPINIT_CB_ID, initCbLambda);
	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPDEINIT_CB_ID, deInitCbLambda);
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
