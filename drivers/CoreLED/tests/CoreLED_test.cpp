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

TEST_F(CoreLEDTests, turnOff)
{
	auto expected_belt_color = RGB::black;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, expected_belt_color.red, expected_belt_color.green,
														expected_belt_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}
	coreled.turnOff();

	ASSERT_EQ(false, coreled.isOn());
}

TEST_F(CoreLEDTests, turnOn)
{
	auto expected_belt_color = RGB::pure_green;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, expected_belt_color.red, expected_belt_color.green,
														expected_belt_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}
	coreled.turnOn();

	ASSERT_EQ(true, coreled.isOn());
}

TEST_F(CoreLEDTests, turnOff1)
{
	auto expected_belt_color = RGB::black;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, expected_belt_color.red, expected_belt_color.green,
														expected_belt_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}
	coreled.turnOff();

	ASSERT_EQ(false, coreled.isOn());
}

TEST_F(CoreLEDTests, turnOn1)
{
	auto expected_belt_color = RGB::pure_green;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, expected_belt_color.red, expected_belt_color.green,
														expected_belt_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}
	coreled.turnOn();

	ASSERT_EQ(true, coreled.isOn());
}

TEST_F(CoreLEDTests, setImplementedColor)
{
	auto expected_belt_color = RGB::pure_red;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, expected_belt_color.red, expected_belt_color.green,
														expected_belt_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}
	coreled.setColor(expected_belt_color);
	coreled.showColor();
}

TEST_F(CoreLEDTests, setUnimplementedBeltColor)
{
	auto expected_belt_color = RGB {255, 255, 0};
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, expected_belt_color.red, expected_belt_color.green,
														expected_belt_color.blue)))
			.Times(20);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00, 0x00, 0x00))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(0x00, 0x00))).Times(1);
	}
	coreled.setColor(expected_belt_color);
	coreled.showColor();
}
