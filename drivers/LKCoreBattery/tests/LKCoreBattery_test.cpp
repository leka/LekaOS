// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreBattery.h"

#include "gtest/gtest.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

LKCoreBattery battery(PinName::BATTERY_VOLTAGE);

float test_set_Voltage(float value)
{
	spy_AnalogIn_setValue(value);
	return value;
}

TEST(LKCoreBatteryTest, initialization)
{
	ASSERT_NE(&battery, nullptr);
}

TEST(LKCoreBatteryTest, readMinVoltage)
{
	auto expected = test_set_Voltage(0.0f);

	ASSERT_EQ(expected, battery.readVoltage());
}

TEST(LKCoreBatteryTest, readMiddleVoltage)
{
	auto expected = test_set_Voltage(0.25f);

	ASSERT_EQ(expected, battery.readVoltage());
}

TEST(LKCoreBatteryTest, readMaxVoltage)
{
	auto expected = test_set_Voltage(1.0f);

	ASSERT_EQ(expected, battery.readVoltage());
}
