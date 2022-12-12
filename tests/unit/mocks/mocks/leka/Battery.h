// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/Battery.h"

namespace leka::mock {

class Battery : public interface::Battery
{
  public:
	Battery();
	virtual ~Battery();

	MOCK_METHOD(void, onChargeDidStart, (mbed::Callback<void()> const &), (override));
	MOCK_METHOD(void, onChargeDidStop, (mbed::Callback<void()> const &), (override));

	MOCK_METHOD(float, voltage, (), (override));
	MOCK_METHOD(uint8_t, level, (), (override));
	MOCK_METHOD(bool, isCharging, (), (override));
};

}	// namespace leka::mock
