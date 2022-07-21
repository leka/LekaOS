// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreTouchSensor.h"

#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/leka/CoreQDAC.h"
#include "mocks/leka/IOExpander.h"

using namespace leka;

using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::SetArrayArgument;

class CoreTouchSensorTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::IOExpander<uint16_t> mockIOExpander {};
	uint8_t pin_number {0x0001};

	leka::io::expanded::DigitalIn<> in {mockIOExpander, pin_number};
	leka::io::expanded::DigitalOut<> out {mockIOExpander, pin_number};
	mock::CoreQDAC dac {};
	Channel channel {Channel::B};

	CoreTouchSensor sensor {in, out, dac, channel};
};

TEST_F(CoreTouchSensorTest, initializationDefault)
{
	EXPECT_CALL(dac, init).Times(1);
	auto new_sensor = CoreTouchSensor {in, out, dac, channel};
	ASSERT_NE(&new_sensor, nullptr);
}

TEST_F(CoreTouchSensorTest, read)
{
	auto expected_read = 0x01;
	EXPECT_CALL(mockIOExpander, readPin(pin_number)).Times(1).WillOnce(Return(expected_read));
	auto actual_read = sensor.read();

	EXPECT_EQ(actual_read, expected_read);
}

TEST_F(CoreTouchSensorTest, setPowerMode)
{
	auto power_mode		= PowerMode::low;
	auto expected_value = static_cast<uint8_t>(power_mode);
	EXPECT_CALL(mockIOExpander, writePin(pin_number, expected_value)).Times(1);
	sensor.setPowerMode(power_mode);
}

TEST_F(CoreTouchSensorTest, setSensitivity)
{
	EXPECT_CALL(dac, write(channel, 0x0ABC, false)).Times(1);
	auto value = 0x0ABC;
	sensor.setSensitivity(value);
}
