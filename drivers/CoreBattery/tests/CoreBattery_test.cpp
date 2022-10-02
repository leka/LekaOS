// Leka - LekaOS
// Copyright 2021-2022 APF rance handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "gtest/gtest.h"
#include "stubs/mbed/AnalogIn.h"
#include "stubs/mbed/InterruptIn.h"

using namespace leka;

class CoreBatteryTest : public ::testing::Test
{
  protected:
	CoreBatteryTest() : battery(PinName::BATTERY_VOLTAGE, charge_input) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mbed::InterruptIn charge_input = {NC};

	CoreBattery battery;
};

TEST_F(CoreBatteryTest, initialization)
{
	ASSERT_NE(&battery, nullptr);
}

TEST_F(CoreBatteryTest, voltageAboveFull)
{
	auto AnalogIn_voltage_value	  = float {2.208};
	auto expected_battery_voltage = float {12.6};
	auto expected_battery_level	  = uint8_t {100};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::full);
	ASSERT_EQ(battery.level(), expected_battery_level);
}

TEST_F(CoreBatteryTest, voltageBetweenThreeQuarterAndFull)
{
	// ? Check again the 3 next value
	auto AnalogIn_voltage_value	  = float {2.135};
	auto expected_battery_voltage = float {11.3};
	auto expected_battery_level	  = uint8_t {88};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::full);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::three_quarter);
	ASSERT_NEAR(battery.level(), expected_battery_level, 5);
}

TEST_F(CoreBatteryTest, voltageBetweenHalfAndThreeQuarter)
{
	// ? Check again the 3 next value
	auto AnalogIn_voltage_value	  = float {2.110};
	auto expected_battery_voltage = float {10.8};
	auto expected_battery_level	  = uint8_t {55};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::three_quarter);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::half);
	ASSERT_NEAR(battery.level(), expected_battery_level, 5);
}

TEST_F(CoreBatteryTest, voltageBetweenQuarterAndHalf)
{
	// ? Check again the 3 next value
	auto AnalogIn_voltage_value	  = float {2.090};
	auto expected_battery_voltage = float {10.6};
	auto expected_battery_level	  = uint8_t {30};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::half);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::quarter);
	ASSERT_NEAR(battery.level(), expected_battery_level, 5);
}

TEST_F(CoreBatteryTest, voltageBetweenEmptyAndQuarter)
{
	auto AnalogIn_voltage_value	  = float {2.033};
	auto expected_battery_voltage = float {9.7};
	auto expected_battery_level	  = uint8_t {15};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::quarter);
	ASSERT_GT(battery.voltage(), CoreBattery::Capacity::empty);
	ASSERT_NEAR(battery.level(), expected_battery_level, 5);
}

TEST_F(CoreBatteryTest, voltageBelowEmpty)
{
	auto AnalogIn_voltage_value	  = float {1.82};
	auto expected_battery_voltage = float {7.5};
	auto expected_battery_level	  = uint8_t {0};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	ASSERT_NEAR(battery.voltage(), expected_battery_voltage, 0.1);
	ASSERT_LT(battery.voltage(), CoreBattery::Capacity::empty);
	ASSERT_EQ(battery.level(), expected_battery_level);
}

TEST_F(CoreBatteryTest, onChargeDidStart)
{
	auto lambda_dummy	 = []() {};
	auto lambda_impostor = []() {};

	mbed::Callback<void()> callback_dummy(lambda_dummy);
	mbed::Callback<void()> callback_impostor(lambda_impostor);

	battery.onChargeDidStart(callback_dummy);

	ASSERT_NE(callback_impostor, callback_dummy);
	ASSERT_NE(callback_impostor, spy_InterruptIn_getRiseCallback());
	ASSERT_EQ(callback_dummy, spy_InterruptIn_getRiseCallback());
}

TEST_F(CoreBatteryTest, onChargeDidStop)
{
	auto lambda_dummy	 = []() {};
	auto lambda_impostor = []() {};

	mbed::Callback<void()> callback_dummy(lambda_dummy);
	mbed::Callback<void()> callback_impostor(lambda_impostor);

	battery.onChargeDidStop(callback_dummy);

	ASSERT_NE(callback_impostor, callback_dummy);
	ASSERT_NE(callback_impostor, spy_InterruptIn_getFallCallback());
	ASSERT_EQ(callback_dummy, spy_InterruptIn_getFallCallback());
}

TEST_F(CoreBatteryTest, isCharging)
{
	spy_InterruptIn_setValue(1);

	ASSERT_TRUE(battery.isCharging());
}

TEST_F(CoreBatteryTest, isNotCharging)
{
	spy_InterruptIn_setValue(0);

	ASSERT_FALSE(battery.isCharging());
}
