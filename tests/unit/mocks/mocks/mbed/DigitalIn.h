// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_MBED_MOCK_DIGITAL_IN_H_
#define _LEKA_OS_MBED_MOCK_DIGITAL_IN_H_

#include "PinNames.h"

#include "drivers/interfaces/InterfaceDigitalIn.h"

#include "gmock/gmock.h"

namespace mbed::mock {

class DigitalIn : public mbed::interface::DigitalIn
{
  public:
	MOCK_METHOD(int, read, (), (override));
	MOCK_METHOD(void, mode, (PinMode pull), (override));
	MOCK_METHOD(int, is_connected, (), (override));
};

}	// namespace mbed::mock

#endif	 // _LEKA_OS_MBED_MOCK_DIGITAL_IN_H_
