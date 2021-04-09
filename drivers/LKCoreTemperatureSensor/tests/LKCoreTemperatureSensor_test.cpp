// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

#include "LKCoreI2C.h"
#include "gtest/gtest.h"
#include "mock_LKCoreI2C.h"
#include "stub_I2C.h"

using ::testing::AnyNumber;
using ::testing::InSequence;

using namespace leka;

namespace lekaI2C {
mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
}	// namespace lekaI2C

class LKCoreI2CTest : public ::testing::Test
{
  protected:
	LKCoreI2CTest() : HTS221_temperatureSensor(i2cMock) {};

	LKCoreTemperatureSensor HTS221_temperatureSensor;
	LKCoreI2CMock i2cMock;
	// TODO: These EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO: Remove them in the future
	void silenceUnexpectedCalls(void)
	{
		EXPECT_CALL(i2cMock, read).Times(AnyNumber());
		EXPECT_CALL(i2cMock, write).Times(AnyNumber());
	};
};

TEST_F(LKCoreI2CTest, initialization)
{
	silenceUnexpectedCalls();
	ASSERT_NE(&HTS221_temperatureSensor, nullptr);
}

TEST_F(LKCoreI2CTest, initializationCountOfReadWrite)
{
	EXPECT_CALL(i2cMock, write).Times(12);
	EXPECT_CALL(i2cMock, read).Times(6);

	HTS221_temperatureSensor.init();
}

TEST_F(LKCoreI2CTest, Calibration)
{
	silenceUnexpectedCalls();
	ASSERT_EQ(HTS221_temperatureSensor.calibration(), status_t::SUCCESS);
}

TEST_F(LKCoreI2CTest, getID)
{
	silenceUnexpectedCalls();
	const int sizeValue	  = 1;
	char value[sizeValue] = {'9'};
	spy_temperatureSensor_setValue(value, sizeValue);
	ASSERT_EQ(HTS221_temperatureSensor.getId(), value[0]);
}

TEST_F(LKCoreI2CTest, getRawTemperature)
{
	silenceUnexpectedCalls();
	const int sizeValue	  = 2;
	char value[sizeValue] = {0x32, 0x00};	// 0x32 = 50
	spy_temperatureSensor_setValue(value, sizeValue);
	int16_t rawTemperatureValue = 50.0;
	ASSERT_EQ(HTS221_temperatureSensor.getRawTemperature(), rawTemperatureValue);
}

TEST_F(LKCoreI2CTest, getRawHumidity)
{
	silenceUnexpectedCalls();
	const int sizeValue	  = 2;
	char value[sizeValue] = {0x32, 0x00};	// 0x32 = 50
	spy_temperatureSensor_setValue(value, sizeValue);
	int16_t rawHumidityValue = 50.0;
	ASSERT_EQ(HTS221_temperatureSensor.getRawHumidity(), rawHumidityValue);
}

TEST_F(LKCoreI2CTest, setHeater)
{
	silenceUnexpectedCalls();
	ASSERT_EQ(HTS221_temperatureSensor.setHeater(state::OFF), status_t::SUCCESS);
	ASSERT_EQ(HTS221_temperatureSensor.setHeater(state::ON), status_t::SUCCESS);
}

TEST_F(LKCoreI2CTest, getHeater)
{
	silenceUnexpectedCalls();
	const int sizeValue	  = 1;
	char value[sizeValue] = {state::OFF};
	spy_temperatureSensor_setValue(value, sizeValue);
	ASSERT_EQ(HTS221_temperatureSensor.getHeater(), state::OFF);
}
TEST_F(LKCoreI2CTest, setIrq)
{
	silenceUnexpectedCalls();
	ASSERT_EQ(HTS221_temperatureSensor.setIrq(state::ON), status_t::SUCCESS);
}
