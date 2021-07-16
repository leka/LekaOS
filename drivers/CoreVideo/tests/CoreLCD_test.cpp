// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLCD.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreLCDDriver.h"
#include "stubs/mbed/PwmOut.h"

using namespace leka;
using ::testing::_;
using ::testing::Args;
using ::testing::AtLeast;
using ::testing::ElementsAre;
using ::testing::InSequence;

class CoreLCDTest : public ::testing::Test
{
  protected:
	CoreLCDTest() : corelcd(lcddrivermock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreLCD corelcd;
	CoreLCDDriverMock lcddrivermock;
};

TEST_F(CoreLCDTest, instantiation)
{
	ASSERT_NE(&corelcd, nullptr);
}

TEST_F(CoreLCDTest, initialization)
{
	{
		InSequence seq;
		EXPECT_CALL(lcddrivermock, initialize).Times(1);
		EXPECT_CALL(lcddrivermock, setLandscapeOrientation).Times(1);
	}

	corelcd.initialize();
}

TEST_F(CoreLCDTest, turnOn)
{
	EXPECT_CALL(lcddrivermock, turnOn).Times(1);

	corelcd.turnOn();
}

TEST_F(CoreLCDTest, turnOff)
{
	EXPECT_CALL(lcddrivermock, turnOff).Times(1);

	corelcd.turnOff();
}

TEST_F(CoreLCDTest, setBrightness)
{
	float any_value;

	EXPECT_CALL(lcddrivermock, setBrightness(any_value)).Times(1);

	corelcd.setBrightness(any_value);
}
