// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreI2C.h"

#include "gtest/gtest.h"
#include "stub_I2C.h"

using namespace leka;

mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreI2C lekaI2C(i2c);

TEST(LKCoreI2CTests, initialisation)
{
	ASSERT_NE(&lekaI2C, nullptr);
}

TEST(LKCoreI2CTests, I2Cread)
{
	char expected[1] = {0x05};
	char readValue[1];
	spy_temperatureSensor_setValue(expected, 1);
	lekaI2C.read(0, readValue, 1, false);
	ASSERT_EQ(readValue[0], expected[0]);
}

TEST(LKCoreI2CTests, I2Cwrite)
{
	char value[1] = {0x09};
	lekaI2C.write(0, value, 1, false);
	char *readValue = spy_temperatureSensor_getValue(1);
	ASSERT_EQ(value[0], readValue[0]);
}
