#include "CoreDAC.h"

namespace leka {

// interface::Dac::DMA_Flag CoreDAC::_dmaFlag = None;

CoreDAC::CoreDAC(LKCoreSTM32HalBase &hal)
	: _hal(hal), _pCallbackCpt(nullptr), _pCallbackHlfCpt(nullptr)	 //, _dmaFlag(None)
{
	_hdac.Instance = DAC;
	_hdma.Instance = DMA1_Stream5;
}

void CoreDAC::initialize()
{
	// GPIO INIT
	//__HAL_RCC_GPIOA_CLK_ENABLE();
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

	/** DAC Initialization
	 */
	//_hdac.Instance = DAC;

	// TODO(aermanio) : handle this problematic situation

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

void CoreDAC::start(uint16_t *pData, uint32_t dataLength)
{
	_hal.HAL_DAC_Start(&_hdac, DAC_CHANNEL_1);	 // TODO(samhadjes): check if this call is really needed
	_hal.HAL_DAC_Start_DMA(&_hdac, DAC_CHANNEL_1, (uint32_t *)pData, dataLength, DAC_ALIGN_12B_R);
}

void CoreDAC::stop()
{
	_hal.HAL_DAC_Stop_DMA(&_hdac, DAC_CHANNEL_1);
}

// auto CoreDAC::dmaFlag() -> DMA_Flag &
// {
// 	return leka::CoreDAC::_dmaFlag;
// }

auto CoreDAC::getHandle() -> DAC_HandleTypeDef &
{
	return this->_hdac;
}

auto CoreDAC::getDMAHandle() -> DMA_HandleTypeDef &
{
	return this->_hdma;
}

void CoreDAC::_registerCallbacks()
{
	// static auto *self = this;
	// _hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_COMPLETE_CB_ID, [](DAC_HandleTypeDef *hdac) {
	// 	// DOESNT WORK WHEN TRYING TO MODIFY AN ATTRIBUTE OF SELF
	// 	self->_cptCallback();
	// });

	// _hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_HALF_COMPLETE_CB_ID, [](DAC_HandleTypeDef *hdac) {
	// 	// DOESNT WORK WHEN TRYING TO MODIFY AN ATTRIBUTE OF SELF
	// 	self->_halfCptCallback();
	// });
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_COMPLETE_CB_ID, _pCallbackCpt);
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_CH1_HALF_COMPLETE_CB_ID, _pCallbackHlfCpt);

	// TODO(aermanio) : check if all callbacks need to be registered
}

// void CoreDAC::_halfCptCallback()
// {
// 	_dmaFlag = Half_cpt;
// }

// void CoreDAC::_cptCallback()
// {
// 	_dmaFlag = Cpt;
// }

void CoreDAC::_registerMspCallbacks()
{
	static auto *self = this;
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSP_INIT_CB_ID,
								  [](DAC_HandleTypeDef *hdac) { self->_mspInitCallback(); });
	_hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSP_DEINIT_CB_ID,
								  [](DAC_HandleTypeDef *hdac) { self->_mspDeInitCallback(); });
}

void CoreDAC::_mspInitCallback()
{
	// DAC MSP INIT
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	_hal.HAL_RCC_DAC_CLK_ENABLE();

	_hal.HAL_RCC_GPIOA_CLK_ENABLE();
	/**DAC GPIO Configuration
	PA4     ------> DAC_OUT1
	*/
	GPIO_InitStruct.Pin	 = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	_hal.HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* DAC DMA Init */
	/* DAC1 Init */
	//_hdma.Instance				   = DMA1_Stream5;
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
	_hdma.Init.MemBurst =
		DMA_MBURST_SINGLE;	 // TODO change to 4 sample burst if single is not longer needed, but the vibration sine
							 // wave creation must be 2 period long in order to have a multiple of 4 in the buffer
	_hdma.Init.PeriphBurst = DMA_PBURST_SINGLE;

	_hal.HAL_DMA_Init(&_hdma);

	__HAL_LINKDMA(&_hdac, DMA_Handle1, _hdma);
}

void CoreDAC::_mspDeInitCallback()
{
	// MSP Deinit
	if (_hdac.Instance == DAC) {
		/* Peripheral clock disable */
		_hal.HAL_RCC_DAC_CLK_DISABLE();

		/**DAC GPIO Configuration
		PA4     ------> DAC_OUT1
		*/
		_hal.HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

		/* DAC DMA DeInit */
		_hal.HAL_DMA_DeInit(_hdac.DMA_Handle1);
	}
}

void CoreDAC::setCptCallbackPtr(pDAC_CallbackTypeDef pCallbackCpt)
{
	this->_pCallbackCpt = pCallbackCpt;
}

void CoreDAC::setHalfCptCallbackPtr(pDAC_CallbackTypeDef pCallbackHlfCpt)
{
	this->_pCallbackHlfCpt = pCallbackHlfCpt;
}

}	// namespace leka

//########################### Overwritten DAC functions #########################################
