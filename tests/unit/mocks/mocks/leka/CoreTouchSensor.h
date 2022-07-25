// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/TouchSensor.h"

namespace leka::mock {

class CoreTouchSensor : public interface::TouchSensor
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(bool, read, (), (override));
	MOCK_METHOD(void, reset, (), (override));
	MOCK_METHOD(void, setSensitivity, (uint16_t, bool), (override));
};

}	// namespace leka::mock
