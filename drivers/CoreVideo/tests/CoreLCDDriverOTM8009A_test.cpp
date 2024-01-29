// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "CoreLCDDriverOTM8009A.hpp"
#include "gtest/gtest.h"
#include "mocks/leka/CoreDSI.h"
#include "mocks/leka/CoreSTM32Hal.h"
#include "stubs/mbed/PwmOut.h"

using namespace leka;

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAre;
using ::testing::InSequence;

class CoreOTM8009ATest : public ::testing::Test
{
  protected:
	CoreOTM8009ATest() : otm(dsimock, PinName::SCREEN_BACKLIGHT_PWM) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreDSI dsimock;
	CoreLCDDriverOTM8009A otm;
};

TEST_F(CoreOTM8009ATest, instantiation)
{
	EXPECT_NE(&otm, nullptr);
}

TEST_F(CoreOTM8009ATest, initialize)
{
	EXPECT_CALL(dsimock, write(_)).Times(101);

	otm.initialize();

	EXPECT_EQ(spy_PwmOut_getPeriod(), 0.01f);
}

TEST_F(CoreOTM8009ATest, setLandscapeOrientation)
{
	auto _settings				 = uint8_t {0x00};
	auto set_landscape_mode		 = [&] { _settings |= (1 << 5); };
	auto set_horizontal_symmetry = [&] { _settings |= (1 << 6); };
	set_landscape_mode();
	set_horizontal_symmetry();

	auto expected_instruction_array = ElementsAre(lcd::otm8009a::madctr::command, _settings);
	auto expected_set_address_for_column_array =
		ElementsAre(0x00, 0x00, 0x03, 0x1F, lcd::otm8009a::set_address::for_column::command);
	auto expected_set_address_for_page_array =
		ElementsAre(0x00, 0x00, 0x01, 0xDF, lcd::otm8009a::set_address::for_page::command);

	{
		InSequence seq;

		EXPECT_CALL(dsimock, write(expected_instruction_array)).Times(1);
		EXPECT_CALL(dsimock, write(expected_set_address_for_column_array)).Times(1);
		EXPECT_CALL(dsimock, write(expected_set_address_for_page_array)).Times(1);
	}

	otm.setLandscapeOrientation();
}

TEST_F(CoreOTM8009ATest, turnOn)
{
	auto expected_instruction_array = ElementsAre(lcd::otm8009a::display::turn_on::command, 0x00);

	EXPECT_CALL(dsimock, write(expected_instruction_array)).Times(1);

	otm.turnOn();
}

TEST_F(CoreOTM8009ATest, turnOff)
{
	auto expected_instruction_array = ElementsAre(lcd::otm8009a::display::turn_off::command, 0x00);

	EXPECT_CALL(dsimock, write(expected_instruction_array)).Times(1);

	otm.turnOff();
}

TEST_F(CoreOTM8009ATest, setBrightness)
{
	otm.setBrightness(0.5);

	EXPECT_EQ(spy_PwmOut_getValue(), 0.5);
}

TEST_F(CoreOTM8009ATest, setBrightnessTurnOffThenTurnOn)
{
	auto initial_brightness_value = 0.4F;

	otm.setBrightness(initial_brightness_value);

	EXPECT_EQ(spy_PwmOut_getValue(), initial_brightness_value);

	EXPECT_CALL(dsimock, write(_)).Times(1);
	otm.turnOff();

	EXPECT_EQ(spy_PwmOut_getValue(), 0);
	EXPECT_NE(spy_PwmOut_getValue(), initial_brightness_value);

	EXPECT_CALL(dsimock, write(_)).Times(1);
	otm.turnOn();

	EXPECT_EQ(spy_PwmOut_getValue(), initial_brightness_value);
}
