// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreI2C.h"

#include "gtest/gtest.h"
#include "stubs/mbed/I2C.h"

using namespace leka;

class CoreI2CTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		expected_read_values	= {};
		actual_read_values		= {};
		expected_written_values = {};
	}
	// void TearDown() override {}

	CoreI2C corei2c {NC, NC};

	std::vector<uint8_t> expected_read_values;
	std::vector<uint8_t> actual_read_values;
	std::vector<uint8_t> expected_written_values;
};

TEST_F(CoreI2CTest, initialisation)
{
	EXPECT_NE(&corei2c, nullptr);
}

TEST_F(CoreI2CTest, writeZeroBytes)
{
	expected_written_values = {};

	corei2c.write(0, expected_written_values.data(), 0, false);

	EXPECT_EQ(expected_written_values, spy_temperatureSensor_getValuesAfterWrite());
}

TEST_F(CoreI2CTest, writeOneByte)
{
	expected_written_values = {0x01};

	corei2c.write(0, expected_written_values.data(), 1, false);

	EXPECT_EQ(expected_written_values, spy_temperatureSensor_getValuesAfterWrite());
}

TEST_F(CoreI2CTest, writeTwoBytes)
{
	expected_written_values = {0x01, 0x02};

	corei2c.write(0, expected_written_values.data(), 2, false);

	EXPECT_EQ(expected_written_values, spy_temperatureSensor_getValuesAfterWrite());
}

TEST_F(CoreI2CTest, writeFiveBytes)
{
	expected_written_values = {0x01, 0x02, 0x03, 0x04, 0x05};

	corei2c.write(0, expected_written_values.data(), 5, false);

	EXPECT_EQ(expected_written_values, spy_temperatureSensor_getValuesAfterWrite());
}

TEST_F(CoreI2CTest, writeTenBytes)
{
	expected_written_values = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

	corei2c.write(0, expected_written_values.data(), 10, false);

	EXPECT_EQ(expected_written_values, spy_temperatureSensor_getValuesAfterWrite());
}

TEST_F(CoreI2CTest, readZeroBytes)
{
	expected_read_values = {};
	actual_read_values.resize(0);
	spy_temperatureSensor_setValuesBeforeRead(expected_read_values);

	corei2c.read(0, actual_read_values.data(), 0, false);

	EXPECT_EQ(expected_read_values, actual_read_values);
}

TEST_F(CoreI2CTest, readOneByte)
{
	expected_read_values = {0x01};
	actual_read_values.resize(1);
	spy_temperatureSensor_setValuesBeforeRead(expected_read_values);

	corei2c.read(0, actual_read_values.data(), 1, false);

	EXPECT_EQ(expected_read_values, actual_read_values);
}

TEST_F(CoreI2CTest, readTwoBytes)
{
	expected_read_values = {0x01, 0x02};
	actual_read_values.resize(2);
	spy_temperatureSensor_setValuesBeforeRead(expected_read_values);

	corei2c.read(0, actual_read_values.data(), 2, false);

	EXPECT_EQ(expected_read_values, actual_read_values);
}

TEST_F(CoreI2CTest, readFiveBytes)
{
	expected_read_values = {0x01, 0x02, 0x03, 0x04, 0x05};
	actual_read_values.resize(5);
	spy_temperatureSensor_setValuesBeforeRead(expected_read_values);

	corei2c.read(0, actual_read_values.data(), 5, false);

	EXPECT_EQ(expected_read_values, actual_read_values);
}

TEST_F(CoreI2CTest, readTenBytes)
{
	expected_read_values = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
	actual_read_values.resize(10);
	spy_temperatureSensor_setValuesBeforeRead(expected_read_values);

	corei2c.read(0, actual_read_values.data(), 10, false);

	EXPECT_EQ(expected_read_values, actual_read_values);
}
