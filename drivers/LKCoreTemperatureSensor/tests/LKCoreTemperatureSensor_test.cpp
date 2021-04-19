// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

#include "LKCoreI2C.h"
#include "gtest/gtest.h"
#include "mock_LKCoreI2C.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

using namespace leka;

class LKCoreTemperatureSensorTest : public ::testing::Test
{
  protected:
	LKCoreTemperatureSensorTest() : HTS221_temperatureSensor(i2cMock) {};

	LKCoreTemperatureSensor HTS221_temperatureSensor;
	LKCoreI2CMock i2cMock;

	void writeSequence()
	{
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
	};
};

TEST_F(LKCoreTemperatureSensorTest, initialization)
{
	ASSERT_NE(&HTS221_temperatureSensor, nullptr);
}

TEST_F(LKCoreTemperatureSensorTest, initializationSequence)
{
	{
		InSequence initSeq;
		for (int i = 0; i < 6; i++) {
			writeSequence();
		}
		uint8_t t0deg[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0deg, t0deg + 1), Return(0)));
		uint8_t t0msbdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0msbdeg, t0msbdeg + 1), Return(0)));
		uint8_t t1deg[1] = {0x08};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1deg, t1deg + 1), Return(0)));
		uint8_t t1mbsdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1mbsdeg, t1mbsdeg + 1), Return(0)));
		uint8_t t0Out[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0Out, t0Out + 2), Return(0)));
		uint8_t t1Out[2] = {0x01, 0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1Out, t1Out + 2), Return(0)));
		uint8_t h0rH[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0rH, h0rH + 1), Return(0)));
		uint8_t h1rH[1] = {0x02};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1rH, h1rH + 1), Return(0)));
		uint8_t h0t0[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0t0, h0t0 + 2), Return(0)));
		uint8_t h1t0[2] = {0x01, 0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1t0, h1t0 + 2), Return(0)));
	}
	HTS221_temperatureSensor.init();

	LKCoreTemperatureSensor::Calibration expected_calibration {1, 0, 1, 0};

	LKCoreTemperatureSensor::Calibration sensor_calibration = HTS221_temperatureSensor.getCalibration();

	ASSERT_EQ(sensor_calibration.humidity.slope, expected_calibration.humidity.slope);
	ASSERT_EQ(sensor_calibration.humidity.y_intercept, expected_calibration.humidity.y_intercept);

	ASSERT_EQ(sensor_calibration.temperature.slope, expected_calibration.temperature.slope);
	ASSERT_EQ(sensor_calibration.temperature.y_intercept, expected_calibration.temperature.y_intercept);
}

TEST_F(LKCoreTemperatureSensorTest, getTemperature)
{
	LKCoreTemperatureSensor::calibration_t temperature_calibration {.temperature.slope		 = 1,
																	.temperature.y_intercept = 0};

	HTS221_temperatureSensor.setCalibration(temperature_calibration);
	uint8_t degC_expected = 0x0A;

	EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(degC_expected), Return(0)));

	ASSERT_EQ(HTS221_temperatureSensor.getTemperature(), degC_expected);
}

TEST_F(LKCoreTemperatureSensorTest, getHumidity)
{
	LKCoreTemperatureSensor::calibration_t humidity_calibration {.humidity.slope = 1, .humidity.y_intercept = 0};

	HTS221_temperatureSensor.setCalibration(humidity_calibration);
	uint8_t rH_expected = 0x0A;

	EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(rH_expected), Return(0)));

	ASSERT_EQ(HTS221_temperatureSensor.getHumidity(), rH_expected);
}
