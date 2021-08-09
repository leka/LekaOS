// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_DAC_H_
#define _LEKA_OS_INTERFACE_DRIVER_DAC_H_

#include <functional>

#include "stm32f7xx_hal.h"

namespace leka::interface {

class DACDriver
{
  public:
	~DACDriver() = default;

	virtual void initialize()								 = 0;
	virtual void terminate()								 = 0;
	virtual void start(uint16_t *pData, uint32_t dataLength) = 0;
	virtual void stop()										 = 0;

	virtual auto getHandle() -> DAC_HandleTypeDef &	   = 0;
	virtual auto getDMAHandle() -> DMA_HandleTypeDef & = 0;

	virtual void setOnHalfBufferReadPtr(pDAC_CallbackTypeDef pCallbackHlfCpt) = 0;
	virtual void setOnFullBufferReadPtr(pDAC_CallbackTypeDef pCallbackCpt)	  = 0;

  protected:
	virtual void _registerCallbacks() = 0;

	virtual void _registerMspCallbacks() = 0;
	virtual void _mspInitCallback()		 = 0;
	virtual void _mspDeInitCallback()	 = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_DAC_H_
