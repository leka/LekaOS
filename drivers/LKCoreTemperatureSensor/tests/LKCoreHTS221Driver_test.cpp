// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreHTS221Driver.h"

#include "gtest/gtest.h"
#include "stub_I2C.h"

using namespace leka;

mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreHTS221Driver temperatureSensor(i2c);

TEST(LKCoreHTS221DriverTest, initialization)
{
	ASSERT_NE(&temperatureSensor, nullptr);
}
