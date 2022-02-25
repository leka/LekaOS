// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "PinNames.h"

#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "gmock/gmock.h"

namespace mbed::mock {

class DigitalOut : public mbed::interface::DigitalOut
{
  public:
	MOCK_METHOD(void, write, (int value), (override));
	MOCK_METHOD(int, read, (), (override));
	MOCK_METHOD(int, is_connected, (), (override));
};

}	// namespace mbed::mock
