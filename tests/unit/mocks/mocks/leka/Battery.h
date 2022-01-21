// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BATTERY_MOCK_H_
#define _LEKA_OS_BATTERY_MOCK_H_

#include "gmock/gmock.h"
#include "interface/drivers/Battery.h"

namespace leka::mock {

class Battery : public interface::Battery
{
  public:
	MOCK_METHOD(float, getVoltage, (), (override));
	MOCK_METHOD(uint8_t, getLevel, (), (override));
	MOCK_METHOD(bool, isCharging, (), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_BATTERY_MOCK_H_
