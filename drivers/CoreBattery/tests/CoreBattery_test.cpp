// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "gtest/gtest.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

CoreBattery corebattery(PinName::BATTERY_VOLTAGE);

float test_set_Voltage(float value)
{
	spy_AnalogIn_setValue(value);
	return value;
}

TEST(CoreBatteryTest, initialization)
{
	ASSERT_NE(&corebattery, nullptr);
}

TEST(CoreBatteryTest, readMinVoltage)
{
	auto expected = test_set_Voltage(0.0f);

	ASSERT_EQ(expected, corebattery.readVoltage());
}

TEST(CoreBatteryTest, readMiddleVoltage)
{
	auto expected = test_set_Voltage(0.25f);

	ASSERT_EQ(expected, corebattery.readVoltage());
}

TEST(CoreBatteryTest, readMaxVoltage)
{
	auto expected = test_set_Voltage(1.0f);

	ASSERT_EQ(expected, corebattery.readVoltage());
}
