// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_H_
#define _LEKA_OS_LIB_DAC_H_

#include "LKCoreSTM32HalBase.h"
#include "CoreDACTimer.h"
#include "interface/drivers/Dac.h"


namespace leka {

// DAC class made to be used with the DACTimer component
// This class configures a DMA channel by default in order to be able to output data through DAC at a high sampling rate without too much impact on performance 
class CoreDAC : public interface::Dac
{
	//TODO(aermanio) : Add final specifiers 
  public:

	explicit CoreDAC(LKCoreSTM32HalBase &hal);

	void initialize() final;
	void deInitialize() final;
	void start(uint16_t* pData, uint32_t dataLength) final;
	void stop() final;

	auto dmaFlag() -> DMA_Flag& final;

	auto getHandle() -> DAC_HandleTypeDef& final;
	
	static DMA_Flag _dmaFlag;
	DMA_Flag _flag;

  protected:
	LKCoreSTM32HalBase &_hal;
    DAC_HandleTypeDef _hdac;
    DMA_HandleTypeDef _hdma;

	void _registerCallbacks() final;
	void _halfCptCallback(DAC_HandleTypeDef *hdac, DMA_Flag* fl) final;
	void _cptCallback(DAC_HandleTypeDef *hdac) final;

	void _registerMspCallbacks() final;
	void _mspInitCallback() final;
	void _mspDeInitCallback() final;	
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_H_
