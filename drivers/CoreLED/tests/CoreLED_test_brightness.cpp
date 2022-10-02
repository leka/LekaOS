// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "CoreSPI.h"
#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;

class CoreLedBrightnessTest : public ::testing::Test
{
  protected:
	CoreLedBrightnessTest() = default;

	void SetUp() override { std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessDefault); }
	// void TearDown() override {}

	static constexpr int number_of_leds		 = 20;
	static constexpr auto kBrightnessDefault = 0xF0;
	static constexpr auto kBrightnessMinimum = 0xE0;

	std::array<uint8_t, number_of_leds> expected_brightness {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<number_of_leds> leds {spi};
};

TEST_F(CoreLedBrightnessTest, setBrightnessForAll)
{
	std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessDefault);

	leds.setColor(RGB::pure_green);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessAtIndexFirst)
{
	auto index = 0;
	auto color = RGB::pure_green;

	expected_brightness.at(index) = kBrightnessDefault;

	leds.setColorAtIndex(index, RGB::pure_green);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessAtIndexMiddle)
{
	auto index = number_of_leds / 2 - 1;
	auto color = RGB::pure_green;

	expected_brightness.at(index) = kBrightnessDefault;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessAtIndexLast)
{
	auto index = number_of_leds - 1;
	auto color = RGB::pure_green;

	expected_brightness.at(index) = kBrightnessDefault;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessAtIndexEqualNumberOfLeds)
{
	auto index = number_of_leds;
	auto color = RGB::pure_green;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessAtIndexHigherThanNumberOfLeds)
{
	auto index = number_of_leds + 100;
	auto color = RGB::pure_green;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessAtIndexFirstMiddleEnd)
{
	auto first_index  = 0;
	auto middle_index = number_of_leds / 2 - 1;
	auto end_index	  = number_of_leds - 1;

	auto first_color  = RGB::pure_red;
	auto middle_color = RGB::pure_green;
	auto end_color	  = RGB::pure_blue;

	expected_brightness.at(first_index)	 = kBrightnessDefault;
	expected_brightness.at(middle_index) = kBrightnessDefault;
	expected_brightness.at(end_index)	 = kBrightnessDefault;

	leds.setColorAtIndex(first_index, first_color);
	leds.setColorAtIndex(middle_index, middle_color);
	leds.setColorAtIndex(end_index, end_color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessFromArray)
{
	auto expected_colors = std::to_array<RGB>(
		{RGB::pure_blue,  RGB::pure_green, RGB::pure_red,  RGB::pure_blue, RGB::yellow,	 RGB::cyan,	   RGB::magenta,
		 RGB::pure_green, RGB::pure_red,   RGB::pure_blue, RGB::yellow,	   RGB::cyan,	 RGB::magenta, RGB::pure_green,
		 RGB::pure_red,	  RGB::pure_blue,  RGB::yellow,	   RGB::cyan,	   RGB::magenta, RGB::black});

	std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessDefault);

	leds.setColorWithArray(expected_colors);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessRange)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 2;
	int range_end_index	  = 5;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_brightness.at(i) = kBrightnessDefault;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessRangeFirstLast)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = number_of_leds - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_brightness.at(i) = kBrightnessDefault;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessRangeFirstMiddle)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = number_of_leds / 2 - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_brightness.at(i) = kBrightnessDefault;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessRangeMiddleLast)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_brightness.at(i) = kBrightnessDefault;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessRangeMiddleLastEqualNumberOfLeds)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds;

	for (auto i = range_begin_index; i < number_of_leds; ++i) {
		expected_brightness.at(i) = kBrightnessDefault;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, setBrightnessRangeMiddleLastHigherThanNumberOfLeds)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds + 100;

	for (auto i = range_begin_index; i < number_of_leds; ++i) {
		expected_brightness.at(i) = kBrightnessDefault;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, fadeToBlackByOneFromDefaultBrightness)
{
	std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessDefault - 1);

	leds.fadeToBlackBy(1);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, fadeToBlackByRandomFromDefaultBrightness)
{
	std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessDefault - 4);

	leds.fadeToBlackBy(4);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, fadeToBlackToMuchFromDefaultBrightness)
{
	std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessMinimum);

	leds.fadeToBlackBy(20);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}

TEST_F(CoreLedBrightnessTest, fadeToBlackAndSetColor)
{
	std::fill(expected_brightness.begin(), expected_brightness.end(), kBrightnessDefault);

	leds.fadeToBlackBy(100);
	leds.setColor(RGB::pure_green);

	EXPECT_TRUE(std::equal(expected_brightness.begin(), expected_brightness.end(), leds.getBrightness().begin()));
}
