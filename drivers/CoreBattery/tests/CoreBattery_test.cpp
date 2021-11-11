// Leka - LekaOS
// Copyright 2021 APF rance handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "gtest/gtest.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

CoreBattery corebattery(PinName::BATTERY_VOLTAGE);

TEST(CoreBatteryTest, initialization)
{
	ASSERT_NE(&corebattery, nullptr);
}

TEST(CoreBatteryTest, getVoltage)
{
	auto AnalogIn_read_voltage_value = float {1.2};
	spy_AnalogIn_setVoltageValue(AnalogIn_read_voltage_value);

	auto expected = AnalogIn_read_voltage_value / CoreBattery::voltage::divider;

	ASSERT_FLOAT_EQ(expected, corebattery.getVoltage());
}
