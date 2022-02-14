// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "CoreSPI.h"
#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;

class CoreLedSetBrightnessTest : public ::testing::Test
{
  protected:
	CoreLedSetBrightnessTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_leds = 20;

	std::array<bRGB, number_of_leds> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<number_of_leds> leds {spi};
};

TEST_F(CoreLedSetBrightnessTest, setRandomBrightnessForAll)
{
	auto led = bRGB {210, RGB::pure_red};

	std::fill(expected_colors.begin(), expected_colors.end(), led);

	leds.setLeds(RGB::pure_red);
	leds.setBrightness(210);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, setDirectlyRandomBrightnessForAll)
{
	auto led = bRGB {210, RGB::pure_red};

	std::fill(expected_colors.begin(), expected_colors.end(), led);

	leds.setLeds(RGB::pure_red, 210);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, setPredefinedBrightnessForAll)
{
	auto led = bRGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), led);

	leds.setLeds(led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, reduceBrightnessbyOne)
{
	auto color = RGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), bRGB {0xEF, color});

	leds.setLeds(color);
	leds.reduceBrightnessBy(1);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, reduceBrightnessbyTwo)
{
	auto color = RGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), bRGB {0xEE, color});

	leds.setLeds(color);
	leds.reduceBrightnessBy(2);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, reduceBrightnessbyARandomNumber)
{
	auto color = RGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), bRGB {0xE3, color});

	leds.setLeds(color);
	leds.reduceBrightnessBy(13);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, reduceBrightnessThenSetColor)
{
	auto color = RGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), bRGB {0xF0, color});

	leds.reduceBrightnessBy(13);
	leds.setLeds(color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetBrightnessTest, reduceBrightnessOverTheLimit)
{
	auto color = RGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), bRGB {0xE0, color});

	leds.setLeds(color);
	leds.reduceBrightnessBy(100);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}
