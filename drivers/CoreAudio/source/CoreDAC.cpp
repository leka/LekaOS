#include "CoreDAC.h"

namespace leka {

CoreDAC::CoreDAC(LKCoreSTM32HalBase &hal)
	: _hal(hal), _pCallbackCpt(nullptr), _pCallbackHlfCpt(nullptr)	 //, _dmaFlag(None)
{
	_hdac.Instance = DAC;
	_hdma.Instance = DMA1_Stream5;
}

void CoreDAC::initialize()
{
	// GPIO INIT
	_hal.HAL_RCC_GPIOA_CLK_ENABLE();

	// DMA INIT
	/* DMA controller clock enable */
	_hal.HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream5_IRQn interrupt configuration */
	/** @brief NVIC configuration for DMA1 stream 5 interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 3, 0);	  // TODO : check that prioority level is ok
	_hal.HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	// DAC INIT
	DAC_ChannelConfTypeDef sConfig = {0};

	_registerMspCallbacks();   // MSP callbacks

	_hal.HAL_DAC_Init(&_hdac);	 // DAC init

	_registerCallbacks();	// classic interrup callback

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger		 = DAC_TRIGGER_T6_TRGO;	  // configure the DAC to be triggered by TIM6 through TRGO signal
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;	  // TODO(aermanio): check if need to enable or not seems to
														  // change output voltage behavior
	_hal.HAL_DAC_ConfigChannel(&_hdac, &sConfig, DAC_CHANNEL_1);
}

void CoreDAC::deInitialize()
{
	_hal.HAL_DAC_DeInit(&_hdac);
}

auto CoreDAC::getHandle() -> DAC_HandleTypeDef &
{
	return this->_hdac;
}

auto CoreDAC::getDMAHandle() -> DMA_HandleTypeDef &
{
	return this->_hdma;
}

}	// namespace leka
