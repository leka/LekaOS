// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLCD.hpp"

#include "gtest/gtest.h"
#include "mocks/leka/CoreLCDDriver.h"

using namespace leka;

using ::testing::An;
using ::testing::InSequence;

class CoreLCDTest : public ::testing::Test
{
  protected:
	CoreLCDTest() : corelcd(lcddrivermock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreLCD corelcd;
	mock::CoreLCDDriver lcddrivermock;
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
	EXPECT_CALL(lcddrivermock, setBrightness(An<float>())).Times(1);

	corelcd.setBrightness(42.F);
}
