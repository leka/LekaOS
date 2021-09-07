// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LCD_MOCK_H_
#define _LEKA_OS_LCD_MOCK_H_

#include "gmock/gmock.h"
#include "interface/LCD.hpp"

namespace leka {

class CoreLCDMock : public interface::LCD
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, turnOn, (), (override));
	MOCK_METHOD(void, turnOff, (), (override));
	MOCK_METHOD(void, setBrightness, (float value), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_LCD_MOCK_H_
