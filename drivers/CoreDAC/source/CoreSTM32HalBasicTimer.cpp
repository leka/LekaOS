// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreSTM32HalBasicTimer.h"
#include <cmath>

#include "LogKit.h"

using namespace leka;

CoreSTM32HalBasicTimer::CoreSTM32HalBasicTimer(interface::STM32Hal &hal) : _hal(hal)
{
	_htim.Instance = TIM6;
}

auto CoreSTM32HalBasicTimer::getHandle() -> TIM_HandleTypeDef &
{
	return _htim;
}

void CoreSTM32HalBasicTimer::registerCallback(std::function<void()> const &callback)
{
	_callback = callback;
}

void CoreSTM32HalBasicTimer::initialize(float frequency)
{
	_registerMspCallbacks();

	// CK_Timer = CK_INT / ((Prescaler + 1) * (Period + 1))
	const auto CK_INT = float {108'000'000.0};
	auto divider	  = static_cast<int>(std::round(CK_INT / frequency));

	_htim.Init.Prescaler		 = 0;
	_htim.Init.Period			 = divider - 1;	  // ? min 1
	_htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	_hal.HAL_TIM_Base_Init(&_htim);

	auto _calculatePeriod = [](uint32_t frequency) {
		uint32_t clockFreq = HAL_RCC_GetPCLK1Freq();

		/* Get PCLK1 prescaler */
		if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0) {
			clockFreq *= 2;
		}

		if (frequency < (clockFreq >> 16) || frequency > clockFreq) {
			log_error("Chosen freq out of bounds\n");
		}

		return (clockFreq / frequency);
	};
	auto periodCalculated = _calculatePeriod(frequency);

	log_info("divider: %ld, prescaler: %ld, calculated: %ld", divider, _htim.Init.Prescaler, periodCalculated);

	auto timerMasterConfig				  = TIM_MasterConfigTypeDef {};
	timerMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	timerMasterConfig.MasterSlaveMode	  = TIM_MASTERSLAVEMODE_DISABLE;
	_hal.HAL_TIMEx_MasterConfigSynchronization(&_htim, &timerMasterConfig);

	// static const auto &self = *this;
	// _hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_PERIOD_ELAPSED_CB_ID, []([[maybe_unused]] TIM_HandleTypeDef *htim)
	// { 	if (self._callback != nullptr) { 		self._callback();
	// 	}
	// });
}

void CoreSTM32HalBasicTimer::_registerMspCallbacks()
{
	static const auto &self = *this;

	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPINIT_CB_ID, []([[maybe_unused]] TIM_HandleTypeDef *htim) {
		self._hal.HAL_RCC_TIM6_CLK_ENABLE();

		self._hal.HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0x00, 0x00);
		self._hal.HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	});

	_hal.HAL_TIM_RegisterCallback(&_htim, HAL_TIM_BASE_MSPDEINIT_CB_ID, []([[maybe_unused]] TIM_HandleTypeDef *htim) {
		self._hal.HAL_RCC_TIM6_CLK_DISABLE();
	});
}

void CoreSTM32HalBasicTimer::linkDACTimer(DAC_ChannelConfTypeDef *config)
{
	if (config != nullptr) {
		config->DAC_Trigger = DAC_TRIGGER_T6_TRGO;
	}
}

void CoreSTM32HalBasicTimer::terminate()
{
	_hal.HAL_TIM_Base_DeInit(&_htim);
}

void CoreSTM32HalBasicTimer::start()
{
	_hal.HAL_TIM_Base_Start_IT(&_htim);
}

void CoreSTM32HalBasicTimer::stop()
{
	_hal.HAL_TIM_Base_Stop_IT(&_htim);
}
