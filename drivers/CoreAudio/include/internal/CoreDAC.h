// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_H_
#define _LEKA_OS_LIB_DAC_H_

#include "CoreDACTimer.h"
#include "LKCoreSTM32HalBase.h"
#include "lstd_span"

namespace leka {
/**
 * @brief DAC class
 * This class interfaces the hardware DAC with its default DMA channel in order to output data
 * at a high sampling rate without too much impact on performance.
 * The class uses a DACTimer as its trigger to output data
 */
class CoreDAC
{
  public:
	explicit CoreDAC(LKCoreSTM32HalBase &hal);

	void initialize();
	void terminate();
	void start(lstd::span<uint16_t> &outBuffer);
	void stop();
	auto getHandle() -> DAC_HandleTypeDef &;
	auto getDMAHandle() -> DMA_HandleTypeDef &;

	void setOnHalfBufferReadPtr(pDAC_CallbackTypeDef pCallbackHlfCpt);
	void setOnFullBufferReadPtr(pDAC_CallbackTypeDef pCallbackCpt);

  private:
	LKCoreSTM32HalBase &_hal;
	DAC_HandleTypeDef _hdac;
	DMA_HandleTypeDef _hdma;

	pDAC_CallbackTypeDef _pOnHalfBufferRead = nullptr;
	pDAC_CallbackTypeDef _pOnFullBufferRead = nullptr;

	void _registerCallbacks();

	void _registerMspCallbacks();
	void _mspInitCallback();
	void _mspDeInitCallback();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_H_
