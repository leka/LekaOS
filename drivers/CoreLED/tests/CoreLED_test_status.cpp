// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "CoreSPI.h"
#include "gtest/gtest.h"

using namespace leka;

class CoreLedStatusTest : public ::testing::Test
{
  protected:
	CoreLedStatusTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_leds = 20;

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<number_of_leds> leds {spi};
};

TEST_F(CoreLedStatusTest, isOffAtInstanciation)
{
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfColorSetToBlack)
{
	leds.setColor(RGB::black);
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfColorIsSetToNotBlack)
{
	leds.setColor(RGB::pure_blue);
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfShowAtInstanciation)
{
	leds.show();
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfSetToBlackAndShow)
{
	leds.setColor(RGB::black);
	leds.show();
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOnIfSetToNotBlackAndShow)
{
	leds.setColor(RGB::pure_blue);
	leds.show();
	EXPECT_TRUE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfSetToBlackAndHide)
{
	leds.setColor(RGB::pure_blue);
	leds.hide();
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfSetToNotBlackAndShowThenHide)
{
	leds.setColor(RGB::pure_blue);
	leds.show();
	leds.hide();
	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfShowBlackAndSetToNotBlack)
{
	leds.setColor(RGB::black);
	leds.show();
	leds.setColor(RGB::pure_blue);

	EXPECT_FALSE(leds.isOn());
}

TEST_F(CoreLedStatusTest, isOffIfShowNotBlackAndSetToBlack)
{
	leds.setColor(RGB::pure_blue);
	leds.show();
	leds.setColor(RGB::black);

	EXPECT_TRUE(leds.isOn());
}
