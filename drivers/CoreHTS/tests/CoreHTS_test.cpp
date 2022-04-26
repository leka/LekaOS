// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "CoreHTS.h"
#include "CoreI2C.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

using namespace leka;
using namespace leka::utils::math;

class CoreHTSTest : public ::testing::Test
{
  protected:
	CoreHTSTest() : corehts(mocki2c) {};

	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t register_reset_value {0x00};
	const uint8_t i2c_address {0xBF};

	CoreHTS corehts;
	mock::CoreI2C mocki2c;
};

TEST_F(CoreHTSTest, initialization)
{
	ASSERT_NE(&corehts, nullptr);
}

TEST_F(CoreHTSTest, initCheckNumberOfCalls)
{
	EXPECT_CALL(mocki2c, write).Times(22);
	EXPECT_CALL(mocki2c, read).Times(16);

	corehts.init();
}

TEST_F(CoreHTSTest, checkI2CAdress)
{
	EXPECT_CALL(mocki2c, write(i2c_address, _, _, _)).Times(22);
	EXPECT_CALL(mocki2c, read(i2c_address, _, _, _)).Times(16);

	corehts.init();
}

TEST_F(CoreHTSTest, writeFailed)
{
	InSequence seq;
	{
		// This write will ask the device to send information.
		EXPECT_CALL(mocki2c, write).Times(1);
		EXPECT_CALL(mocki2c, read).Times(1);

		// This write will change the value inside a device's register.
		EXPECT_CALL(mocki2c, write).WillOnce(Return(1));
	}

	corehts.turnOn();
}

TEST_F(CoreHTSTest, turnOnSequenceAndValue)
{
	auto expected_values = ElementsAre(0xA0, 0x80);

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(i2c_address, Pointee(HTS221_CTRL_REG1 | 0x80), _, _)).Times(1);
		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
	}

	corehts.turnOn();
}

TEST_F(CoreHTSTest, enableBlockDataUpdateSequenceAndValue)
{
	auto expected_values = ElementsAre(0xA0, 0x04);

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_CTRL_REG1 | 0x80), _, _)).Times(1);
		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
	}
	corehts.enableBlockDataUpdate();
}

TEST_F(CoreHTSTest, setDataAquisitionRateSequenceAndValue)
{
	auto expected_values = ElementsAre(0xA0, 0x02);

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_CTRL_REG1 | 0x80), _, _)).Times(1);
		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
	}

	corehts.setDataAcquisitionRate();
}

TEST_F(CoreHTSTest, turnOffHeaterSequenceAndValue)
{
	auto expected_values = ElementsAre(0xA1, 0x00);

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_CTRL_REG2 | 0x80), _, _)).Times(1);
		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
	}

	corehts.turnOffHeater();
}

TEST_F(CoreHTSTest, setNumberOfMeasurementsToCalculateAverageTemperatureSequenceAndValue)
{
	auto expected_values = ElementsAre(0x90, 0x18);

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_AV_CONF | 0x80), _, _)).Times(1);
		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
	}

	corehts.setNumberOfMeasurementsToCalculateAverageTemperature();
}

TEST_F(CoreHTSTest, setNumberOfMeasurementsToCalculateAverageHumiditySequenceAndValue)
{
	auto expected_values = ElementsAre(0x90, 0x03);

	InSequence seq;
	{
		EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_AV_CONF | 0x80), _, _)).Times(1);
		EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(register_reset_value), Return(0)));
		EXPECT_CALL(mocki2c, write).With(Args<1, 2>(expected_values));
	}

	corehts.setNumberOfMeasurementsToCalculateAverageHumidity();
}

TEST_F(CoreHTSTest, getCalibrationPointsForHumidity)
{
	EXPECT_CALL(mocki2c, read).Times(AnyNumber());

	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_H0_T0_OUT_L | 0x80), _, _)).Times(1);
	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_H1_T0_OUT_L | 0x80), _, _)).Times(1);
	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_H0_RH_X2 | 0x80), _, _)).Times(1);
	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_H1_RH_X2 | 0x80), _, _)).Times(1);

	corehts.getCalibrationPointsForHumidity();
}

TEST_F(CoreHTSTest, failedGetCalibrationPointsForHumidity)
{
	auto expected = Point {0, 0};

	EXPECT_CALL(mocki2c, write).WillRepeatedly(Return(1));

	auto [p1, p2] = corehts.getCalibrationPointsForHumidity();

	ASSERT_EQ(p1.x, expected.x);
	ASSERT_EQ(p2.y, expected.y);
}

TEST_F(CoreHTSTest, failedGetCalibrationPointsForTemperature)
{
	auto expected = Point {0, 0};

	EXPECT_CALL(mocki2c, write).WillRepeatedly(Return(1));

	auto [p1, p2] = corehts.getCalibrationPointsForTemperature();

	ASSERT_EQ(p1.x, expected.x);
	ASSERT_EQ(p2.y, expected.y);
}

TEST_F(CoreHTSTest, getCalibrationPointsForTemperature)
{
	Sequence seqt0deg, seqt1deg;

	EXPECT_CALL(mocki2c, read).Times(AnyNumber());

	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_T0_OUT_L | 0x80), _, _)).Times(1).InSequence(seqt0deg);
	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_T0_DEGC_X8 | 0x80), _, _)).Times(1).InSequence(seqt0deg);

	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_T1_OUT_L | 0x80), _, _)).Times(1).InSequence(seqt1deg);
	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_T1_DEGC_X8 | 0x80), _, _)).Times(1).InSequence(seqt1deg);

	EXPECT_CALL(mocki2c, write(_, Pointee(HTS221_T1_T0_MSB | 0x80), _, _)).Times(2);

	corehts.getCalibrationPointsForTemperature();
}

TEST_F(CoreHTSTest, getTemperatureCalibrationCoefficients)
{
	LinearCoefficients expected {.slope = 1, .y_intercept = 2};

	corehts.setLinearCalibrationForTemperature(expected);

	auto actual = corehts.getTemperatureCalibrationCoefficients();

	ASSERT_EQ(actual.slope, expected.slope);
	ASSERT_EQ(actual.y_intercept, expected.y_intercept);
}

TEST_F(CoreHTSTest, getHumidityCalibrationCoefficients)
{
	LinearCoefficients expected {.slope = 1, .y_intercept = 2};

	corehts.setLinearCalibrationForHumidity(expected);

	auto actual = corehts.getHumidityCalibrationCoefficients();

	ASSERT_EQ(actual.slope, expected.slope);
	ASSERT_EQ(actual.y_intercept, expected.y_intercept);
}

TEST_F(CoreHTSTest, calibrateTemperatureCheckNumberOfCalls)
{
	EXPECT_CALL(mocki2c, write).Times(6);
	EXPECT_CALL(mocki2c, read).Times(6);

	corehts.calibrateTemperature();
}

TEST_F(CoreHTSTest, calibrateHumidityCheckNumberOfCalls)
{
	EXPECT_CALL(mocki2c, write).Times(4);
	EXPECT_CALL(mocki2c, read).Times(4);

	corehts.calibrateHumidity();
}

TEST_F(CoreHTSTest, getTemperature)
{
	auto degC_expected = std::to_array<uint8_t>({100, 0x00});
	LinearCoefficients coefficients {.slope = 1, .y_intercept = 0};

	corehts.setLinearCalibrationForTemperature(coefficients);

	EXPECT_CALL(mocki2c, write).Times(1);
	EXPECT_CALL(mocki2c, read)
		.WillOnce(DoAll(SetArrayArgument<1>(std::begin(degC_expected), std::end(degC_expected)), Return(0)));

	auto degC_actual = corehts.getTemperatureCelsius();

	ASSERT_EQ(degC_actual, static_cast<float>(degC_expected[0]));
}

TEST_F(CoreHTSTest, getTemperatureFailed)
{
	float degC_expected = -42;
	LinearCoefficients coefficients {.slope = 1, .y_intercept = 0};

	corehts.setLinearCalibrationForTemperature(coefficients);

	EXPECT_CALL(mocki2c, write).Times(1);
	EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(0), Return(1)));

	auto degC_actual = corehts.getTemperatureCelsius();

	ASSERT_EQ(degC_actual, degC_expected);
}

TEST_F(CoreHTSTest, getHumidity)
{
	auto rH_expected = std::to_array<uint8_t>({0x0A, 0x00});
	LinearCoefficients coefficients {.slope = 1, .y_intercept = 0};

	corehts.setLinearCalibrationForHumidity(coefficients);

	EXPECT_CALL(mocki2c, write).Times(1);
	EXPECT_CALL(mocki2c, read)
		.WillOnce(DoAll(SetArrayArgument<1>(std::begin(rH_expected), std::end(rH_expected)), Return(0)));

	auto rh_actual = corehts.getRelativeHumidity();

	ASSERT_EQ(rh_actual, rH_expected[0]);
}

TEST_F(CoreHTSTest, getHumidityFailed)
{
	float rH_expected = -42;
	LinearCoefficients coefficients {.slope = 1, .y_intercept = 0};

	corehts.setLinearCalibrationForHumidity(coefficients);

	EXPECT_CALL(mocki2c, write).Times(1);
	EXPECT_CALL(mocki2c, read).WillOnce(DoAll(SetArgPointee<1>(0), Return(1)));

	auto rh_actual = corehts.getRelativeHumidity();

	ASSERT_EQ(rh_actual, rH_expected);
}
