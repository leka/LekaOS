// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DAC_H_
#define _LEKA_OS_LIB_DAC_H_

#include "CoreDACTimer.h"
#include "LKCoreSTM32HalBase.h"
#include "interface/drivers/DACDriver.h"

namespace leka {
/**
 * @brief DAC class
 * This class interfaces the hardware DAC with its default DMA channel in order to output data
 * at a high sampling rate without too much impact on performance.
 * The class uses a DACTimer as its trigger to output data
 */
class CoreDAC : public interface::DACDriver
{
  public:
	explicit CoreDAC(LKCoreSTM32HalBase &hal);

	void initialize() final;
	void terminate() final;
	void start(uint16_t *pData, uint32_t dataLength) final;
	void stop() final;

	auto getHandle() -> DAC_HandleTypeDef & final;
	auto getDMAHandle() -> DMA_HandleTypeDef & final;

	void setOnHalfBufferReadPtr(pDAC_CallbackTypeDef pCallbackHlfCpt) final;
	void setOnFullBufferReadPtr(pDAC_CallbackTypeDef pCallbackCpt) final;

  private:
	LKCoreSTM32HalBase &_hal;
	DAC_HandleTypeDef _hdac;
	DMA_HandleTypeDef _hdma;

	pDAC_CallbackTypeDef _pOnHalfBufferRead = nullptr;
	pDAC_CallbackTypeDef _pOnFullBufferRead = nullptr;

	void _registerCallbacks() final;

	void _registerMspCallbacks() final;
	void _mspInitCallback() final;
	void _mspDeInitCallback() final;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DAC_H_
