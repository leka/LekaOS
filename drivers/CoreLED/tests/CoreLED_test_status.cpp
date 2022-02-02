// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;
using ::testing::NiceMock;

class CoreLEDTestsStatus : public ::testing::Test
{
  protected:
	CoreLEDTestsStatus() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_belt_leds = 20;
	NiceMock<mock::SPI> spimock {};
	CoreLED<number_of_belt_leds> belt {spimock};
};

TEST_F(CoreLEDTestsStatus, isOffAtInstanciation)
{
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfColorSetToBlack)
{
	belt.setColor(RGB::black);
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfColorIsSetToNotBlack)
{
	belt.setColor(RGB::pure_blue);
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfShowAtInstanciation)
{
	belt.showColor();
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfSetToBlackAndShow)
{
	belt.setColor(RGB::black);
	belt.showColor();
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOnIfSetToNotBlackAndShow)
{
	belt.setColor(RGB::pure_blue);
	belt.showColor();
	EXPECT_TRUE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfSetToBlackAndHide)
{
	belt.setColor(RGB::pure_blue);
	belt.hideColor();
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfSetToNotBlackAndShowThenHide)
{
	belt.setColor(RGB::pure_blue);
	belt.showColor();
	belt.hideColor();
	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfShowBlackAndSetToNotBlack)
{
	belt.setColor(RGB::black);
	belt.showColor();
	belt.setColor(RGB::pure_blue);

	EXPECT_FALSE(belt.isOn());
}

TEST_F(CoreLEDTestsStatus, isOffIfShowNotBlackAndSetToBlack)
{
	belt.setColor(RGB::pure_blue);
	belt.showColor();
	belt.setColor(RGB::black);

	EXPECT_TRUE(belt.isOn());
}
