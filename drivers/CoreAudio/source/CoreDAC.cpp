#include "CoreDAC.h"

namespace leka 
{

CoreDAC::CoreDAC(LKCoreSTM32HalBase &hal) : _hal(hal), _dmaFlag(None){}

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
	_hal.HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 3, 0); // TODO : check that prioority level is ok
	_hal.HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    // DAC INIT
    DAC_ChannelConfTypeDef sConfig = {0};

	/** DAC Initialization
	 */
	_hdac.Instance = DAC;

    //TODO(aermanio) : handle this problematic situation
    _registerCallbacks();

	_hal.HAL_DAC_Init(&_hdac) ;

    //DAC MSP INIT
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
                _hdma.Instance				   = DMA1_Stream5;
                _hdma.Init.Channel			   = DMA_CHANNEL_7;
                _hdma.Init.Direction		   = DMA_MEMORY_TO_PERIPH;
                _hdma.Init.PeriphInc		   = DMA_PINC_DISABLE;
                _hdma.Init.MemInc			   = DMA_MINC_ENABLE;
                _hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
                _hdma.Init.MemDataAlignment	   = DMA_MDATAALIGN_HALFWORD;
                _hdma.Init.Mode				   = DMA_CIRCULAR;
                _hdma.Init.Priority			   = DMA_PRIORITY_LOW;		// changed from low to high
                _hdma.Init.FIFOMode			   = DMA_FIFOMODE_ENABLE;	// enable
                _hdma.Init.FIFOThreshold	   = DMA_FIFO_THRESHOLD_HALFFULL;
                _hdma.Init.MemBurst			   = DMA_MBURST_INC4;	// inc4
                _hdma.Init.PeriphBurst		   = DMA_PBURST_SINGLE;

                _hal.HAL_DMA_Init(&_hdma);

                _hal.HAL_LINKDMA_DAC(&_hdac, _hdac.DMA_Handle1 , _hdma);
                
    /** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger		 = DAC_TRIGGER_T6_TRGO; // configure the DAC to be triggered by TIM6 through TRGO signal
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;	  // TODO(aermanio): check if need to enable or not seems to change output voltage behavior

	_hal.HAL_DAC_ConfigChannel(&_hdac, &sConfig, DAC_CHANNEL_1);

}

void CoreDAC::deInitialize()
{
    HAL_DAC_DeInit(&_hdac);
	
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

void CoreDAC::start(uint16_t* pData, uint32_t dataLength)
{
    _hal.HAL_DAC_Start(&_hdac, DAC_CHANNEL_1);  // TODO(samhadjes): check if this call is really needed
	_hal.HAL_DAC_Start_DMA(&_hdac, DAC_CHANNEL_1, (uint32_t*) pData, dataLength, DAC_ALIGN_12B_R);
}

void CoreDAC::stop()
{
    _hal.HAL_DAC_Stop_DMA(&_hdac, DAC_CHANNEL_1);
}

auto CoreDAC::getHandle() -> DAC_HandleTypeDef*
{
    return &(this->_hdac);
}

void CoreDAC::_registerCallbacks()
{
    static auto *self = this;
    _hal.HAL_DAC_RegisterCallback(&_hdac,  HAL_DAC_CH1_COMPLETE_CB_ID, [](DAC_HandleTypeDef *hdac){ self->_cptCallback();});
    _hal.HAL_DAC_RegisterCallback(&_hdac,  HAL_DAC_CH1_HALF_COMPLETE_CB_ID, [](DAC_HandleTypeDef *hdac){ self->_halfCptCallback();});

    _hal.HAL_DAC_RegisterCallback(&_hdac, HAL_DAC_MSP_INIT_CB_ID, [](DAC_HandleTypeDef *hdac) { self->_mspInitCallback();});

    // TODO IF ERROR CHECK BY REGISTERING ALL CALLBACKS
}

void CoreDAC::_halfCptCallback()
{
    //TODO Put flag to true if in polling mode
    _dmaFlag = Half_cpt;
}

void CoreDAC::_cptCallback()
{
    //TODO Put flag to true if in polling mode
    _dmaFlag = Cpt;
}

void CoreDAC::_mspInitCallback()
{

}



} // namespace leka

//########################### Overwritten DAC functions #########################################

