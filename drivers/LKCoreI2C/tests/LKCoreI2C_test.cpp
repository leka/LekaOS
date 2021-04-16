// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreI2C.h"

#include "gtest/gtest.h"
#include "stub_I2C.h"

using namespace leka;

mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreI2C corei2c(i2c);

TEST(LKCoreI2CTests, initialisation)
{
	ASSERT_NE(&corei2c, nullptr);
}

TEST(LKCoreI2CTests, writeTwoValues)
{
	auto expected_written_values = std::vector<char> {0x08, 0x09};
	corei2c.write(0, expected_written_values.data(), std::size(expected_written_values), false);

	auto actual_written_values = spy_temperatureSensor_getValue();
	ASSERT_EQ(expected_written_values, actual_written_values);
}

TEST(LKCoreI2CTests, readTwoValues)
{
	auto expected_read_values = std::vector<char> {0x01, 0x02};
	auto actual_read_values	  = std::vector<char> {0x00, 0x00};
	spy_temperatureSensor_setValue(expected_read_values);
	corei2c.read(0, actual_read_values.data(), std::size(actual_read_values), false);

	ASSERT_EQ(expected_read_values, actual_read_values);
}
