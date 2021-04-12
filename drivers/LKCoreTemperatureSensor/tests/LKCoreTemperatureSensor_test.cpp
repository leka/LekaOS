// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

#include "LKCoreI2C.h"
#include "gtest/gtest.h"
#include "mock_LKCoreI2C.h"
// #include "stub_I2C.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

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
	EXPECT_CALL(i2cMock, write).Times(10);
	EXPECT_CALL(i2cMock, read).Times(10);

	ASSERT_EQ(HTS221_temperatureSensor.calibration(), status_t::SUCCESS);
}

TEST_F(LKCoreI2CTest, GetTemperature)
{
	// init values for calibration
	{
		InSequence seq;
		uint8_t t0deg[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0deg, t0deg + 1), Return(0)));
		uint8_t t0msbdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0msbdeg, t0msbdeg + 1), Return(0)));
		uint8_t t1deg[1] = {0x40};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1deg, t1deg + 1), Return(0)));
		uint8_t t1mbsdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1mbsdeg, t1mbsdeg + 1), Return(0)));
		uint8_t t0Out[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0Out, t0Out + 2), Return(0)));
		uint8_t t1Out[2] = {0x01, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1Out, t1Out + 2), Return(0)));
		uint8_t h0rH[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0rH, h0rH + 1), Return(0)));
		uint8_t h1rH[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1rH, h1rH + 1), Return(0)));
		uint8_t h0t0[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0t0, h0t0 + 2), Return(0)));
		uint8_t h1t0[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1t0, h1t0 + 2), Return(0)));
	}

	ASSERT_EQ(HTS221_temperatureSensor.calibration(), status_t::SUCCESS);
	uint8_t expected = 0x0A;   // expected value for temperature in degC
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(expected), Return(0)));
	ASSERT_EQ(HTS221_temperatureSensor.getTemperature(), expected);
}

TEST_F(LKCoreI2CTest, GetHumidity)
{
	// init values for calibration
	{
		InSequence seq;
		uint8_t t0deg[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0deg, t0deg + 1), Return(0)));
		uint8_t t0msbdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0msbdeg, t0msbdeg + 1), Return(0)));
		uint8_t t1deg[1] = {0x40};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1deg, t1deg + 1), Return(0)));
		uint8_t t1mbsdeg[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1mbsdeg, t1mbsdeg + 1), Return(0)));
		uint8_t t0Out[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t0Out, t0Out + 2), Return(0)));
		uint8_t t1Out[2] = {0x01, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(t1Out, t1Out + 2), Return(0)));
		uint8_t h0rH[1] = {0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0rH, h0rH + 1), Return(0)));
		uint8_t h1rH[1] = {0x04};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1rH, h1rH + 1), Return(0)));
		uint8_t h0t0[2] = {0x00, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0t0, h0t0 + 2), Return(0)));
		uint8_t h1t0[2] = {0x01, 0x00};
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1t0, h1t0 + 2), Return(0)));
	}
	ASSERT_EQ(HTS221_temperatureSensor.calibration(), status_t::SUCCESS);
	uint8_t expected = 0x0A;   // expected value for humidty in rH
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(expected), Return(0)));
	ASSERT_EQ(HTS221_temperatureSensor.getTemperature(), expected);
}
