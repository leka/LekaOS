// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_H_
#define _LEKA_OS_LIB_DAC_H_

#include "LKCoreSTM32HalBase.h"
#include "CoreDACTimer.h"

// redeclared DAC functions
extern "C" {
void DMA1_Stream5_IRQHandler();
}
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac);
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac);


namespace leka {

// DAC class made to be used with the DACTimer component
// This class configures a DMA channel by default in order to be able to output data through DAC at a high sampling rate without too much impact on performance 
class CoreDAC
{
	//TODO(aermanio) : Add final specifiers 
  public:
	enum DMA_Flag
	{
		None,
		Half_cpt,
		Cpt
	};


	explicit CoreDAC(LKCoreSTM32HalBase &hal);

	void initialize();
	void deInitialize();
	void start(uint16_t* pData, uint32_t dataLength);
	void stop();

	auto dmaFlag() -> DMA_Flag& {return _dmaFlag;}

	auto getHandle() -> DAC_HandleTypeDef&;
	DMA_Flag _dmaFlag;


	void _registerCallbacks();

	void test(){_x += 1;}

  private:
	LKCoreSTM32HalBase &_hal;
    DAC_HandleTypeDef _hdac;
    DMA_HandleTypeDef _hdma;
	void _halfCptCallback(DAC_HandleTypeDef *hdac);
	void _cptCallback(DAC_HandleTypeDef *hdac);

	void _registerMspCallbacks();
	void _mspInitCallback();
	void _mspDeInitCallback();

  protected :
	uint8_t _x;

	
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_H_
