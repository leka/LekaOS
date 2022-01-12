// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_MICROPHONE_H_
#define _LEKA_OS_DRIVER_LK_CORE_MICROPHONE_H_

#include "drivers/AnalogIn.h"

namespace leka {

class CoreMicrophone
{
  public:
	explicit CoreMicrophone(PinName pin) : _pin {mbed::AnalogIn(pin)} {};

	float readVolume(void);

  private:
	mbed::AnalogIn _pin;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_MICROPHONE_H_
