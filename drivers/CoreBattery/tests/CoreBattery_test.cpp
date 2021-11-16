// Leka - LekaOS
// Copyright 2021 APF rance handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "gtest/gtest.h"
#include "mocks/mbed/DigitalIn.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

using ::testing::Return;

class CoreBatteryTest : public ::testing::Test
{
  protected:
	CoreBatteryTest() : battery(PinName::BATTERY_VOLTAGE, charge_input) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mbed::mock::DigitalIn charge_input = {};

	CoreBattery battery;
};

TEST_F(CoreBatteryTest, initialization)
{
	ASSERT_NE(&battery, nullptr);
}

TEST_F(CoreBatteryTest, getVoltageAboveFull)
{
	auto AnalogIn_voltage_value	  = float {2.208};
	auto expected_battery_voltage = float {12.6};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.getVoltage(), expected_battery_voltage, 0.1);
	ASSERT_GT(battery.getVoltage(), CoreBattery::Capacity::full);
}

TEST_F(CoreBatteryTest, getVoltageBetweenThreeQuarterAndFull)
{
	auto AnalogIn_voltage_value	  = float {2.183};
	auto expected_battery_voltage = float {12.1};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.getVoltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.getVoltage(), CoreBattery::Capacity::full);
	ASSERT_GT(battery.getVoltage(), CoreBattery::Capacity::three_quarter);
}

TEST_F(CoreBatteryTest, getVoltageBetweenHalfAndThreeQuarter)
{
	auto AnalogIn_voltage_value	  = float {2.144};
	auto expected_battery_voltage = float {11.4};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.getVoltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.getVoltage(), CoreBattery::Capacity::three_quarter);
	ASSERT_GT(battery.getVoltage(), CoreBattery::Capacity::half);
}

TEST_F(CoreBatteryTest, getVoltageBetweenQuarterAndHalf)
{
	auto AnalogIn_voltage_value	  = float {2.108};
	auto expected_battery_voltage = float {10.8};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.getVoltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.getVoltage(), CoreBattery::Capacity::half);
	ASSERT_GT(battery.getVoltage(), CoreBattery::Capacity::quarter);
}

TEST_F(CoreBatteryTest, getVoltageBetweenEmptyAndQuarter)
{
	auto AnalogIn_voltage_value	  = float {2.033};
	auto expected_battery_voltage = float {9.7};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.getVoltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.getVoltage(), CoreBattery::Capacity::quarter);
	ASSERT_GT(battery.getVoltage(), CoreBattery::Capacity::empty);
}

TEST_F(CoreBatteryTest, getVoltageBelowEmpty)
{
	auto AnalogIn_voltage_value	  = float {1.82};
	auto expected_battery_voltage = float {7.5};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.getVoltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.getVoltage(), CoreBattery::Capacity::empty);
}

TEST_F(CoreBatteryTest, isInCharge)
{
	EXPECT_CALL(charge_input, read).WillOnce(Return(1));

	ASSERT_TRUE(battery.isInCharge());
}

TEST_F(CoreBatteryTest, isNotInCharge)
{
	EXPECT_CALL(charge_input, read).WillOnce(Return(0));

	ASSERT_FALSE(battery.isInCharge());
}
