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
	// TODO: These EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO: Remove them in the future
	void silenceUnexpectedCalls(void)
	{
		EXPECT_CALL(i2cMock, read).Times(AnyNumber());
		EXPECT_CALL(i2cMock, write).Times(AnyNumber());
	};
};

TEST_F(LKCoreTemperatureSensorTest, initialization)
{
	silenceUnexpectedCalls();
	ASSERT_NE(&HTS221_temperatureSensor, nullptr);
}

TEST_F(LKCoreTemperatureSensorTest, initializationSequence)
{
	{
		InSequence initSeq;
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
	}
	HTS221_temperatureSensor.init();
}

TEST_F(LKCoreTemperatureSensorTest, Calibration)
{
	{
		InSequence seq;
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

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

	LKCoreTemperatureSensor::Calibration expectedCalibrationValues;
	expectedCalibrationValues.is_initialise			  = true;
	expectedCalibrationValues.humidity.slope		  = 1;
	expectedCalibrationValues.humidity.y_intercept	  = 0;
	expectedCalibrationValues.temperature.slope		  = 1;
	expectedCalibrationValues.temperature.y_intercept = 0;

	HTS221_temperatureSensor.init();
	LKCoreTemperatureSensor::Calibration CalibrationValues = HTS221_temperatureSensor.getCalibration();

	ASSERT_EQ(CalibrationValues.is_initialise, expectedCalibrationValues.is_initialise);
	ASSERT_EQ(CalibrationValues.humidity.slope, expectedCalibrationValues.humidity.slope);
	ASSERT_EQ(CalibrationValues.humidity.y_intercept, expectedCalibrationValues.humidity.y_intercept);
	ASSERT_EQ(CalibrationValues.temperature.slope, expectedCalibrationValues.temperature.slope);
	ASSERT_EQ(CalibrationValues.temperature.y_intercept, expectedCalibrationValues.temperature.y_intercept);
}

TEST_F(LKCoreTemperatureSensorTest, GetTemperature)
{
	{
		InSequence seq;
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

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
		uint8_t h1rH[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1rH, h1rH + 1), Return(0)));
		uint8_t h0t0[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0t0, h0t0 + 2), Return(0)));
		uint8_t h1t0[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1t0, h1t0 + 2), Return(0)));
	}

	HTS221_temperatureSensor.init();
	uint8_t degCExpected = 0x0A;
	EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(degCExpected), Return(0)));
	ASSERT_EQ(HTS221_temperatureSensor.getTemperature(), degCExpected);
}

TEST_F(LKCoreTemperatureSensorTest, GetHumidity)
{
	{
		InSequence seq;
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);

		uint8_t t0deg[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0deg, t0deg + 1), Return(0)));
		uint8_t t0msbdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0msbdeg, t0msbdeg + 1), Return(0)));
		uint8_t t1deg[1] = {0x00};
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
	uint8_t rHExpected = 0x0A;
	EXPECT_CALL(i2cMock, write(_, _, _, _)).Times(1);
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(rHExpected), Return(0)));
	ASSERT_EQ(HTS221_temperatureSensor.getHumidity(), rHExpected);
}
