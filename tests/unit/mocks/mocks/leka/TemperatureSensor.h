// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/TemperatureSensor.h"

namespace leka::mock {

class TemperatureSensor : public interface::TemperatureSensor
{
  public:
	MOCK_METHOD(float, getTemperatureCelsius, (), (override));
};

class HumiditySensor : public interface::HumiditySensor
{
  public:
	MOCK_METHOD(float, getRelativeHumidity, (), (override));
};

}	// namespace leka::mock
