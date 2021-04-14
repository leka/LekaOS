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

TEST(LKCoreI2CTests, coreI2CWrite)
{
	char value[2] = {0x08, 0x09};

	corei2c.write(0, value, 2, false);
	std::vector<char> read_value = spy_temperatureSensor_getValue(2);

	ASSERT_EQ(value[0], read_value[0]);
	ASSERT_EQ(value[1], read_value[1]);
}

TEST(LKCoreI2CTests, coreI2CRead)
{
	std::vector<char> expected = {char(0x01), char(0x02)};
	char read_value[2];

	spy_temperatureSensor_setValue(expected, 2);
	corei2c.read(0, read_value, 2, false);

	ASSERT_EQ(read_value[0], expected[0]);
	ASSERT_EQ(read_value[1], expected[1]);
}
