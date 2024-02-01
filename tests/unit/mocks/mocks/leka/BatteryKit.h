// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/libs/BatteryKit.h"

namespace leka::mock {

class Battery : public interface::BatteryKit
{
  public:
	MOCK_METHOD(void, startEventHandler, (), (override));

	MOCK_METHOD(uint8_t, level, (), (override));
	MOCK_METHOD(void, onDataUpdated, (std::function<void()> const &), (override));
	MOCK_METHOD(void, onLowBattery, (std::function<void()> const &), (override));

	MOCK_METHOD(bool, isCharging, (), (override));
	MOCK_METHOD(void, onChargeDidStart, (std::function<void()> const &), (override));
	MOCK_METHOD(void, onChargeDidStop, (std::function<void()> const &), (override));
};

}	// namespace leka::mock
