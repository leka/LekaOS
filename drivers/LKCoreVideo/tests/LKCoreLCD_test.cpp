// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLCD.h"

#include "gtest/gtest.h"
#include "mock_LKCoreLCDDriver.h"
#include "stub_PwmOut.h"

using namespace leka;
using ::testing::_;
using ::testing::Args;
using ::testing::AtLeast;
using ::testing::ElementsAre;
using ::testing::InSequence;

class LKCoreLCDTest : public ::testing::Test
{
  protected:
	LKCoreLCDTest() : corelcd(lcddrivermock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreLCD corelcd;
	LKCoreLCDDriverMock lcddrivermock;
};

TEST_F(LKCoreLCDTest, instantiation)
{
	ASSERT_NE(&corelcd, nullptr);
}

TEST_F(LKCoreLCDTest, initialization)
{
	{
		InSequence seq;
		EXPECT_CALL(lcddrivermock, initialize).Times(1);
		EXPECT_CALL(lcddrivermock, setLandscapeOrientation).Times(1);
	}

	corelcd.initialize();
}

TEST_F(LKCoreLCDTest, turnOn)
{
	EXPECT_CALL(lcddrivermock, turnOn).Times(1);

	corelcd.turnOn();
}

TEST_F(LKCoreLCDTest, turnOff)
{
	EXPECT_CALL(lcddrivermock, turnOff).Times(1);

	corelcd.turnOff();
}

TEST_F(LKCoreLCDTest, setBrightness)
{
	float any_value;

	EXPECT_CALL(lcddrivermock, setBrightness(any_value)).Times(1);

	corelcd.setBrightness(any_value);
}
