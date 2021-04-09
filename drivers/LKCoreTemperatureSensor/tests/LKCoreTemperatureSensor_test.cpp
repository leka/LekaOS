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
using ::testing::Args;
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

TEST_F(LKCoreI2CTest, CalibrationAndGetTemperature)
{
	char test;
	// silenceUnexpectedCalls();
	// init values for calibration
	{
		InSequence seq;
		test = 0x00;   // t0degC/8 [7,0]
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
		test = 0x00;   // ????
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
		test = 0x40;   // t1degC/8
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
		test = 0x00;   // t0_Out [15,8]
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
		test = 0x00;   // t0_Out [7,0]
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
		test = 0x01;   // t1_Out
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
		char h0rH[1] = {0x01};	 // h0rH
		// EXPECT_CALL(i2cMock, read(_, _, _, _)).With(Args<1>(humidity)).Times(1);
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0rH, h0rH + 1), Return(0)));
		char h1rH[1] = {0x02};	 // h1rH
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1rH, h1rH + 1), Return(0)));
		// EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(humidity[0]), Return(0)));
		char h0t0[2];
		h0t0[0] = 0x01;	  // h0t0
		h0t0[1] = 0x00;	  // h0t0
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h0t0, h0t0 + 1), Return(0)));
		// EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(humidity[0]), Return(0)));
		char h1t0[2];
		h1t0[0] = 0x06;	  // h1t0
		h1t0[1] = 0x00;	  // h1t0
		EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArrayArgument<1>(h1t0, h1t0 + 1), Return(0)));
		// EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(humidity[0]), Return(0)));
	}
	ASSERT_EQ(HTS221_temperatureSensor.calibration(), status_t::SUCCESS);
	test = 0x0A;   // value for temperature
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
	ASSERT_EQ(HTS221_temperatureSensor.getTemperature(), test);

	printf("Value of slope %f and Zero %f\n", HTS221_temperatureSensor.getSlope(), HTS221_temperatureSensor.getZero());
	test = 0x0A;   // value for humidity
	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(test), Return(0)));
	ASSERT_EQ(HTS221_temperatureSensor.getHumidity(), test);
}

// TEST_F(LKCoreI2CTest, getTemperature)
// {
// 	silenceUnexpectedCalls();
// 	char test[1] = {0x0A};
// 	EXPECT_CALL(i2cMock, read(_, _, _, _)).WillOnce(DoAll(SetArgPointee<1>(*test), Return(0)));
// 	printf("Value of slope %f and Zero %f\n", HTS221_temperatureSensor.getSlope(), HTS221_temperatureSensor.getZero());
// 	ASSERT_EQ(HTS221_temperatureSensor.getTemperature(), 1);
// }

TEST_F(LKCoreI2CTest, getHumidity)
{
	silenceUnexpectedCalls();
	ASSERT_EQ(HTS221_temperatureSensor.getHumidity(), 0);
}
