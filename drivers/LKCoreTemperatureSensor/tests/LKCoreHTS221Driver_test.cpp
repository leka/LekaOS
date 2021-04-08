// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreHTS221Driver.h"

#include "LKCoreTemperatureSensor.h"
#include "gtest/gtest.h"
#include "stub_I2C.h"

using namespace leka;

mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreHTS221Driver HTS221_temperatureSensor(i2c);

TEST(LKCoreHTS221DriverTest, initialization)
{
	ASSERT_NE(&HTS221_temperatureSensor, nullptr);
}

TEST(LKCoreHTS221DriverTest, HTS221Init)
{
	ASSERT_EQ(HTS221_temperatureSensor.init(), status_t::SUCCESS);
}

TEST(LKCoreHTS221DriverTest, Calibration)
{
	ASSERT_EQ(HTS221_temperatureSensor.calibration(), status_t::SUCCESS);
}

TEST(LKCoreHTS221DriverTest, getID)
{
	const int sizeValue	  = 1;
	char value[sizeValue] = {'9'};
	spy_temperatureSensor_setValue(value, sizeValue);
	ASSERT_EQ(HTS221_temperatureSensor.getId(), value[0]);
}

TEST(LKCoreHTS221DriverTest, getRawTemperature)
{
	const int sizeValue	  = 2;
	char value[sizeValue] = {0x32, 0x00};	// 0x32 = 50
	spy_temperatureSensor_setValue(value, sizeValue);
	int16_t rawTemperatureValue = 50.0;
	ASSERT_EQ(HTS221_temperatureSensor.getRawTemperature(), rawTemperatureValue);
}

TEST(LKCoreHTS221DriverTest, getRawHumidity)
{
	const int sizeValue	  = 2;
	char value[sizeValue] = {0x32, 0x00};	// 0x32 = 50
	spy_temperatureSensor_setValue(value, sizeValue);
	int16_t rawHumidityValue = 50.0;
	ASSERT_EQ(HTS221_temperatureSensor.getRawHumidity(), rawHumidityValue);
}

TEST(LKCoreHTS221DriverTest, setHeater)
{
	ASSERT_EQ(HTS221_temperatureSensor.setHeater(state::OFF), status_t::SUCCESS);
	ASSERT_EQ(HTS221_temperatureSensor.setHeater(state::ON), status_t::SUCCESS);
}

TEST(LKCoreHTS221DriverTest, getHeater)
{
	const int sizeValue	  = 1;
	char value[sizeValue] = {state::OFF};
	spy_temperatureSensor_setValue(value, sizeValue);
	ASSERT_EQ(HTS221_temperatureSensor.getHeater(), state::OFF);
}
TEST(LKCoreHTS221DriverTest, setIrq)
{
	ASSERT_EQ(HTS221_temperatureSensor.setIrq(state::ON), status_t::SUCCESS);
}

LKCoreTemperatureSensor temperatureSensor(i2c);
TEST(LKCoreTemperatureSensorTest, initialization)
{
	ASSERT_NE(&temperatureSensor, nullptr);
}
