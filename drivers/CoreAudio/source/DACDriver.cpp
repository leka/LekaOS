// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "DACDriver.h"

namespace leka {

DACDriver::DACDriver(LKCoreSTM32HalBase &hal) : _hal(hal)
{
	_hdac.Instance = DAC;
	_hdma.Instance = DMA1_Stream5;	 // DMA1_Stream5 is the only DMA channel for DAC
}

void DACDriver::terminate()
{
	_hal.HAL_DAC_DeInit(&_hdac);
}

void DACDriver::linkNewTimer(const DACTimer &tim)
{
	DAC_ChannelConfTypeDef sConfig = {0};
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;	  // necessary to reach the full voltage range in DAC output

	if (tim.getHardWareBasicTimer() == DACTimer::HardwareBasicTimer::BasicTimer6) {
		sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;	 // configure the DAC to be triggered by TIM6 through TRGO signal
	} else if (tim.getHardWareBasicTimer() == DACTimer::HardwareBasicTimer::BasicTimer7) {
		sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;	 // configure the DAC to be triggered by TIM7 through TRGO signal
	}

	_hal.HAL_DAC_ConfigChannel(&_hdac, &sConfig, DAC_CHANNEL_1);
}

void DACDriver::start(lstd::span<uint16_t> outBuffer)
{
	_hal.HAL_DAC_Start_DMA(&_hdac, DAC_CHANNEL_1, (uint32_t *)(outBuffer.data()), outBuffer.size(), DAC_ALIGN_12B_R);
}

void DACDriver::stop()
{
	_hal.HAL_DAC_Stop_DMA(&_hdac, DAC_CHANNEL_1);
}

auto DACDriver::getHandle() const -> const DAC_HandleTypeDef &
{
	return this->_hdac;
}

auto DACDriver::getDMAHandle() const -> const DMA_HandleTypeDef &
{
	return this->_hdma;
}

void DACDriver::_init(const DACTimer &tim)
{
	_hal.HAL_RCC_GPIOA_CLK_ENABLE();

	_hal.HAL_RCC_DMA1_CLK_ENABLE();	  // DAC can only be connected to DMA1

	_hal.HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	_registerMspCallbacks();   // needs to be called before init
	_hal.HAL_DAC_Init(&_hdac);
	_registerInterruptCallbacks();	 // needs to be called after init

	linkNewTimer(tim);
}

void DACDriver::_registerInterruptCallbacks()
{
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_HALF_COMPLETE_CB_ID, _pOnHalfBufferRead);
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_COMPLETE_CB_ID, _pOnFullBufferRead);
}

void DACDriver::_registerMspCallbacks()
{
	static auto *self	= this;
	auto initCbLambda	= []([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_msp_onInitializationCb(); };
	auto deInitCbLambda = []([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_msp_onTerminationCb(); };
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSPINIT_CB_ID, initCbLambda);
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSPDEINIT_CB_ID, deInitCbLambda);
}

void DACDriver::_msp_onInitializationCb()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	_hal.HAL_RCC_DAC_CLK_ENABLE();

	_hal.HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin	 = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	_hal.HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	_hdma.Init.Channel			   = DMA_CHANNEL_7;
	_hdma.Init.Direction		   = DMA_MEMORY_TO_PERIPH;
	_hdma.Init.PeriphInc		   = DMA_PINC_DISABLE;
	_hdma.Init.MemInc			   = DMA_MINC_ENABLE;
	_hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	_hdma.Init.MemDataAlignment	   = DMA_MDATAALIGN_HALFWORD;
	_hdma.Init.Mode				   = DMA_CIRCULAR;
	_hdma.Init.Priority			   = DMA_PRIORITY_LOW;
	_hdma.Init.FIFOMode			   = DMA_FIFOMODE_ENABLE;
	_hdma.Init.FIFOThreshold	   = DMA_FIFO_THRESHOLD_HALFFULL;
	_hdma.Init.MemBurst			   = DMA_MBURST_INC4;
	_hdma.Init.PeriphBurst		   = DMA_PBURST_SINGLE;

	_hal.HAL_DMA_Init(&_hdma);

	__HAL_LINKDMA(&_hdac, DMA_Handle1, _hdma);
}

void DACDriver::_msp_onTerminationCb()
{
	if (_hdac.Instance == DAC) {
		_hal.HAL_RCC_DAC_CLK_DISABLE();

		_hal.HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

		_hal.HAL_DMA_DeInit(_hdac.DMA_Handle1);
	}
}

}	// namespace leka
