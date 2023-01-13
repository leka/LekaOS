// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/LCD.hpp"

namespace leka::mock {

class CoreLCD : public interface::LCD
{
  public:
	CoreLCD();
	virtual ~CoreLCD();

	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, turnOn, (), (override));
	MOCK_METHOD(void, turnOff, (), (override));
	MOCK_METHOD(void, setBrightness, (float value), (override));
};

}	// namespace leka::mock
