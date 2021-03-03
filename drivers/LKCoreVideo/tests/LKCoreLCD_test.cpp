// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLCD.h"

#include "gtest/gtest.h"
#include "mock_LKCoreDSI.h"
#include "stub_PwmOut.h"

using namespace leka;
using ::testing::_;
using ::testing::AtLeast;

// TODO:
// - Check driver init called
// - Check arrays in setLandscapeOrientation

class LKCoreLCDTest : public ::testing::Test
{
  protected:
	LKCoreLCDTest() : corelcd(dsimock, SCREEN_BACKLIGHT_PWM) {}

	void SetUp() override {}
	void TearDown() override {}

	LKCoreDSIMock dsimock;
	LKCoreLCD corelcd;
};

TEST_F(LKCoreLCDTest, instantiation)
{
	ASSERT_NE(&corelcd, nullptr);
}

TEST_F(LKCoreLCDTest, backlightPwmPeriodIsSet)
{
	ASSERT_EQ(spy_PwmOut_getPeriodValue(), 0.01f);
}

TEST_F(LKCoreLCDTest, initialization)
{
	corelcd.initialize();
}

TEST_F(LKCoreLCDTest, turnOn)
{
	EXPECT_CALL(dsimock, writeCommand).Times(AtLeast(1));

	corelcd.turnOn();
}

TEST_F(LKCoreLCDTest, turnOff)
{
	EXPECT_CALL(dsimock, writeCommand).Times(AtLeast(1));

	corelcd.turnOff();
}

TEST_F(LKCoreLCDTest, setBrightnessValue)
{
	float value = 0.42f;
	corelcd.setBrightness(value);

	ASSERT_EQ(spy_PwmOut_getValue(), value);
}

TEST_F(LKCoreLCDTest, setBrightnessNotLowerThanZero)
{
	corelcd.setBrightness(-100);

	ASSERT_EQ(spy_PwmOut_getValue(), 0);
}

TEST_F(LKCoreLCDTest, setBrightnessGreaterThanOne)
{
	corelcd.setBrightness(100);

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
}

TEST_F(LKCoreLCDTest, getCommandByteForLandscapeOrientationVerticalSymmetryTrue)
{
	auto command_byte = corelcd.getCommandByteForLandscapeOrientation(true, false, false, false, false);

	ASSERT_EQ(command_byte, 0x80);
}

TEST_F(LKCoreLCDTest, getCommandByteForLandscapeOrientationHorizontalSymmetryTrue)
{
	auto command_byte = corelcd.getCommandByteForLandscapeOrientation(false, true, false, false, false);

	ASSERT_EQ(command_byte, 0x40);
}

TEST_F(LKCoreLCDTest, getCommandByteForLandscapeOrientationIsLandscapeTrue)
{
	auto command_byte = corelcd.getCommandByteForLandscapeOrientation(false, false, true, false, false);

	ASSERT_EQ(command_byte, 0x20);
}

TEST_F(LKCoreLCDTest, getCommandByteForLandscapeOrientationReverseRefreshTopToBottomTrue)
{
	auto command_byte = corelcd.getCommandByteForLandscapeOrientation(false, false, false, true, false);

	ASSERT_EQ(command_byte, 0x10);
}

TEST_F(LKCoreLCDTest, getCommandByteForLandscapeOrientationUseBGRTrue)
{
	auto command_byte = corelcd.getCommandByteForLandscapeOrientation(false, false, false, false, true);

	ASSERT_EQ(command_byte, 0x08);
}

TEST_F(LKCoreLCDTest, setLandscapeOrientation)
{
	EXPECT_CALL(dsimock, writeCommand(_, 0)).Times(AtLeast(1));
	EXPECT_CALL(dsimock, writeCommand(_, 4)).Times(AtLeast(2));

	corelcd.setLandscapeOrientation();
}
