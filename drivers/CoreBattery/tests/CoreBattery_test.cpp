// Leka - LekaOS
// Copyright 2021 APF rance handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "gtest/gtest.h"
#include "mocks/mbed/DigitalIn.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

using ::testing::Return;

CoreBattery corebattery(PinName::BATTERY_VOLTAGE);

class CoreChargeTest : public ::testing::Test
{
  protected:
	CoreChargeTest() : charge(charge_input) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mbed::mock::DigitalIn charge_input = {};

	CoreCharge charge;
};

TEST(CoreBatteryTest, initialization)
{
	ASSERT_NE(&corebattery, nullptr);
}

TEST(CoreBatteryTest, getVoltage)
{
	auto AnalogIn_voltage_value = float {1.2};
	spy_AnalogIn_setVoltageValue(AnalogIn_voltage_value);

	auto expected_voltage = CoreBattery::polynomial::degree_0 +
							CoreBattery::polynomial::degree_1 * AnalogIn_voltage_value +
							CoreBattery::polynomial::degree_2 * AnalogIn_voltage_value * AnalogIn_voltage_value;

	ASSERT_FLOAT_EQ(expected_voltage, corebattery.getVoltage());
}

// TEST_F(CoreChargeTest, isOn)
// {
// 	EXPECT_CALL(charge_input, read).WillOnce(Return(1));

// 	ASSERT_TRUE(charge.isOn());
// }

// TEST_F(CoreChargeTest, isOff)
// {
// 	EXPECT_CALL(charge_input, read).WillOnce(Return(0));

// 	ASSERT_FALSE(charge.isOn());
// }
