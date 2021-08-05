// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "CoreLCDDriverOTM8009A.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreVideo/CoreDSI.h"
#include "mocks/leka/LKCoreSTM32Hal.h"
#include "stubs/mbed/PwmOut.h"

using namespace leka;

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAre;
using ::testing::InSequence;

class LKCoreOTM8009ATest : public ::testing::Test
{
  protected:
	LKCoreOTM8009ATest() : otm(dsimock, PinName::SCREEN_BACKLIGHT_PWM) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreDSIMock dsimock;
	CoreLCDDriverOTM8009A otm;
};

TEST_F(LKCoreOTM8009ATest, instantiation)
{
	ASSERT_NE(&otm, nullptr);
}

TEST_F(LKCoreOTM8009ATest, initialize)
{
	EXPECT_CALL(dsimock, write).Times(101);

	otm.initialize();

	ASSERT_EQ(spy_PwmOut_getPeriod(), 0.01f);
}

TEST_F(LKCoreOTM8009ATest, setLandscapeOrientation)
{
	auto expected_instruction_array = ElementsAre(lcd::otm8009a::madctr::command, 0xA0);
	auto expected_set_address_for_column_array =
		ElementsAre(0x00, 0x00, 0x03, 0x1F, lcd::otm8009a::set_address::for_column::command);
	auto expected_set_address_for_page_array =
		ElementsAre(0x00, 0x00, 0x01, 0xDF, lcd::otm8009a::set_address::for_page::command);

	{
		InSequence seq;

		EXPECT_CALL(dsimock, write(_, 2)).With(Args<0, 1>(expected_instruction_array)).Times(1);
		EXPECT_CALL(dsimock, write(_, 5)).With(Args<0, 1>(expected_set_address_for_column_array)).Times(1);
		EXPECT_CALL(dsimock, write(_, 5)).With(Args<0, 1>(expected_set_address_for_page_array)).Times(1);
	}

	otm.setLandscapeOrientation();
}

TEST_F(LKCoreOTM8009ATest, turnOn)
{
	auto expected_instruction_array = ElementsAre(lcd::otm8009a::display::turn_on::command, 0x00);

	EXPECT_CALL(dsimock, write(_, 2)).With(Args<0, 1>(expected_instruction_array)).Times(1);

	otm.turnOn();
}

TEST_F(LKCoreOTM8009ATest, turnOff)
{
	auto expected_instruction_array = ElementsAre(lcd::otm8009a::display::turn_off::command, 0x00);

	EXPECT_CALL(dsimock, write(_, 2)).With(Args<0, 1>(expected_instruction_array)).Times(1);

	otm.turnOff();
}

TEST_F(LKCoreOTM8009ATest, setBrightness)
{
	otm.setBrightness(0.5);

	ASSERT_EQ(spy_PwmOut_getValue(), 0.5);
}
