// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_H_
#define _LEKA_OS_LIB_DAC_H_

#include "LKCoreSTM32HalBase.h"
#include "CoreDACTimer.h"

//TODO Add final specifiers 

namespace leka {

// DAC class made to be used with the DACTimer component
// This class configures a DMA channel by default in order to be able to output data through DAC at a high sampling rate without too much impact on performance 
class CoreDAC
{
  public:
	explicit CoreDAC(LKCoreSTM32HalBase &hal);

	void initialize();
	void deInitialize();
	void start();
	void stop();

	auto getHandle() -> DAC_HandleTypeDef;

  private:
	LKCoreSTM32HalBase &_hal;
    DAC_HandleTypeDef _hdac;
    DMA_HandleTypeDef _hdma;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_H_
