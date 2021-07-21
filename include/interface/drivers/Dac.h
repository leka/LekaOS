// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_DAC_H_
#define _LEKA_OS_INTERFACE_DRIVER_DAC_H_

#include "stm32f7xx_hal.h"

namespace leka::interface {

class Dac
{
  public:
	enum DMA_Flag
	{
		None,
		Half_cpt,
		Cpt
	};

	~Dac() = default;

	virtual void initialize()								 = 0;
	virtual void deInitialize()								 = 0;
	virtual void start(uint16_t *pData, uint32_t dataLength) = 0;
	virtual void stop()										 = 0;

	// virtual auto dmaFlag() -> DMA_Flag & = 0;

	virtual auto getHandle() -> DAC_HandleTypeDef &	   = 0;
	virtual auto getDMAHandle() -> DMA_HandleTypeDef & = 0;

	virtual void setCptCallbackPtr(pDAC_CallbackTypeDef pCallbackCpt)	  = 0;
	virtual void setHalfCptCallbackPtr(pDAC_CallbackTypeDef pCallbackCpt) = 0;

  protected:
	virtual void _registerCallbacks() = 0;
	// virtual void _halfCptCallback()	  = 0;
	// virtual void _cptCallback()		  = 0;

	virtual void _registerMspCallbacks() = 0;
	virtual void _mspInitCallback()		 = 0;
	virtual void _mspDeInitCallback()	 = 0;
};

}	// namespace leka::interface

#endif