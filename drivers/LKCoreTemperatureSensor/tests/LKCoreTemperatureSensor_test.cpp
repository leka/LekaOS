// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

#include "gtest/gtest.h"

using namespace leka;

mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreTemperatureSensor temperatureSensor(i2c);

TEST(LKCoreTemperatureSensorTest, initialization)
{
	ASSERT_NE(&temperatureSensor, nullptr);
}
