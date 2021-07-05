// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

namespace leka {

CoreDACTimer::CoreDACTimer(LKCoreSTM32HalBase &hal)
: _hal(hal)
{
}

void CoreDACTimer::initialize(float frequency)
{
    _hal.HAL_RCC_TIM6_CLK_ENABLE();

    _htim.Instance				 = TIM6;    //select timer
    _htim.Init.Prescaler		 = 0;       // no need of prescaler for high frequencies
    _htim.Init.CounterMode		 = TIM_COUNTERMODE_UP;  //count up
    _htim.Init.Period			 = calculatePeriod(frequency);  //period of the timer
    _htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  //no need to change counter period while working
    _hal.HAL_TIM_Base_Init(&_htim);     //TODO : handle errors if necessary
    
    
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;    //trigger used for DAC
    sMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;    //TODO verify utility of this parameter
    _hal.HAL_TIMEx_MasterConfigSynchronization(&_htim, &sMasterConfig);//TODO : handle errors if necessary
}

void CoreDACTimer::start()
{
    _hal.HAL_TIM_Base_Start(&_htim);
}

void CoreDACTimer::stop()
{
    _hal.HAL_TIM_Base_Stop(&_htim);
}

void CoreDACTimer::deInitialize()
{
    _hal.HAL_TIM_Base_DeInit(&_htim);
	__HAL_RCC_TIM6_CLK_DISABLE();
    
    // TODO : do we make a "stop and deInit" method
}

auto CoreDACTimer::getHandle() -> TIM_HandleTypeDef
{
    return this->_htim;
}

auto CoreDACTimer::calculatePeriod(float frequency) -> uint32_t
{
    // TODO : handle frequency in a non-supported range
    // TODO : check if an enum of closed values would be better for allowed frequencies
    uint32_t period;
    uint32_t clockFreq = HAL_RCC_GetPCLK1Freq();    //TODO : make this a HAL method if necessary

    /* Get PCLK1 prescaler */
    if((RCC->CFGR & RCC_CFGR_PPRE1) != 0)
    {
        clockFreq *=2;
    };

    return period = clockFreq / frequency ;
}



































}   //namespace Leka