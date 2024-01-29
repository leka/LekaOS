// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/LCDDriver.hpp"

namespace leka::mock {

class CoreLCDDriver : public interface::LCDDriver
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, setLandscapeOrientation, (), (override));
	MOCK_METHOD(void, turnOn, (), (override));
	MOCK_METHOD(void, turnOff, (), (override));
	MOCK_METHOD(void, setBrightness, (float value), (override));

	MOCK_METHOD(void, enableDeepSleep, (), (override));
	MOCK_METHOD(void, disableDeepSleep, (), (override));
};

}	// namespace leka::mock
