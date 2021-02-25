// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLCD.h"

#include "gtest/gtest.h"
#include "mock_LKCoreDSI.h"
#include "st_otm8009a.h"
#include "stub_PwmOut.h"

using namespace leka;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::ElementsAre;
using ::testing::InSequence;

// TODO:
// - Check driver init called

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

ACTION_TEMPLATE(SaveArray, HAS_3_TEMPLATE_PARAMS(int, k, int, l, typename, T), AND_1_VALUE_PARAMS(pointer))
{
	const T *data		= std::get<k>(args);
	const uint32_t size = std::get<l>(args);

	// TODO: update when writeCommand of DSI is updated
	for (int i = 0; i < size + 1; i++) {
		(*pointer)[i] = *(data + i);
	}
}

TEST_F(LKCoreLCDTest, setLandscapeOrientation)
{
	auto expected_instruction_array	  = ElementsAre(OTM8009A_CMD_MADCTR, 0xA0);
	auto expected_update_column_array = ElementsAre(0x00, 0x00, 0x03, 0x1F, OTM8009A_CMD_CASET);
	auto expected_update_pages_array  = ElementsAre(0x00, 0x00, 0x01, 0xDF, OTM8009A_CMD_PASET);

	uint8_t actual_instruction_array[2];
	uint8_t actual_update_column_array[5];
	uint8_t actual_update_pages_array[5];

	{
		InSequence seq;

		// TODO: update when writeCommand of DSI is updated
		EXPECT_CALL(dsimock, writeCommand(_, 1)).WillOnce(SaveArray<0, 1, uint8_t>(&actual_instruction_array));
		EXPECT_CALL(dsimock, writeCommand(_, 4)).WillOnce(SaveArray<0, 1, uint8_t>(&actual_update_column_array));
		EXPECT_CALL(dsimock, writeCommand(_, 4)).WillOnce(SaveArray<0, 1, uint8_t>(&actual_update_pages_array));
	}

	corelcd.setLandscapeOrientation();

	EXPECT_THAT(actual_instruction_array, expected_instruction_array);
	EXPECT_THAT(actual_update_column_array, expected_update_column_array);
	EXPECT_THAT(actual_update_pages_array, expected_update_pages_array);
}
