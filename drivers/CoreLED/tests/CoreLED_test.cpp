// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;

class CoreLEDTests : public ::testing::Test
{
  protected:
	CoreLEDTests() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::SPI spimock {};
	CoreLED coreled {spimock, 20};
};

TEST_F(CoreLEDTests, initialisation)
{
	ASSERT_NE(&coreled, nullptr);
}

TEST_F(CoreLEDTests, setColorAndShowPredefinedColor)
{
	auto color = RGB::pure_red;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.red, color.green, color.blue))).Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}

	coreled.setColor(color);
	coreled.showColor();
}

TEST_F(CoreLEDTests, setColorAndShowRandomColor)
{
	auto color = RGB {120, 12, 56};

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.red, color.green, color.blue))).Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}

	coreled.setColor(color);
	coreled.showColor();
}

TEST_F(CoreLEDTests, turnOnWithoutSetColor)
{
	auto expected_color = RGB::black;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock,
					write(testing::ElementsAre(_, expected_color.red, expected_color.green, expected_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}

	coreled.turnOn();

	EXPECT_EQ(coreled.isOn(), true);
}

TEST_F(CoreLEDTests, turnOffWithoutSetColor)
{
	auto expected_color = RGB::black;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock,
					write(testing::ElementsAre(_, expected_color.red, expected_color.green, expected_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}

	coreled.turnOff();

	EXPECT_EQ(coreled.isOn(), false);
}
