// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "CoreSPI.h"
#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;

class CoreLedSetColorTest : public ::testing::Test
{
  protected:
	CoreLedSetColorTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_leds = 20;

	std::array<RGB, number_of_leds> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<number_of_leds> leds {spi};
};

TEST_F(CoreLedSetColorTest, setColorPredefinedForAll)
{
	auto color = RGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), color);

	leds.setColor(color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorUserDefinedForAll)
{
	auto color = RGB {120, 12, 56};

	std::fill(expected_colors.begin(), expected_colors.end(), color);

	leds.setColor(color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorAtIndexFirst)
{
	auto index = 0;
	auto color = RGB::pure_green;

	expected_colors.at(index) = color;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorAtIndexMiddle)
{
	auto index = number_of_leds / 2 - 1;
	auto color = RGB::pure_green;

	expected_colors.at(index) = color;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorAtIndexLast)
{
	auto index = number_of_leds - 1;
	auto color = RGB::pure_green;

	expected_colors.at(index) = color;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorAtIndexEqualNumberOfLeds)
{
	auto index = number_of_leds;
	auto color = RGB::pure_green;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorAtIndexHigherThanNumberOfLeds)
{
	auto index = number_of_leds + 100;
	auto color = RGB::pure_green;

	leds.setColorAtIndex(index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorAtIndexFirstMiddleEnd)
{
	auto first_index  = 0;
	auto middle_index = number_of_leds / 2 - 1;
	auto end_index	  = number_of_leds - 1;

	auto first_color  = RGB::pure_red;
	auto middle_color = RGB::pure_green;
	auto end_color	  = RGB::pure_blue;

	expected_colors.at(first_index)	 = first_color;
	expected_colors.at(middle_index) = middle_color;
	expected_colors.at(end_index)	 = end_color;

	leds.setColorAtIndex(first_index, first_color);
	leds.setColorAtIndex(middle_index, middle_color);
	leds.setColorAtIndex(end_index, end_color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorFromArray)
{
	auto expected_colors = std::to_array<RGB>(
		{RGB::pure_blue,  RGB::pure_green, RGB::pure_red,  RGB::pure_blue, RGB::yellow,	 RGB::cyan,	   RGB::magenta,
		 RGB::pure_green, RGB::pure_red,   RGB::pure_blue, RGB::yellow,	   RGB::cyan,	 RGB::magenta, RGB::pure_green,
		 RGB::pure_red,	  RGB::pure_blue,  RGB::yellow,	   RGB::cyan,	   RGB::magenta, RGB::black});

	leds.setColorWithArray(expected_colors);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRange)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 2;
	int range_end_index	  = 5;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeFirstLast)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = number_of_leds - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeFirstMiddle)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = number_of_leds / 2 - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeMiddleLast)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeMiddleLastEqualNumberOfLeds)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds;

	for (auto i = range_begin_index; i < number_of_leds; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeMiddleLastHigherThanNumberOfLeds)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds + 100;

	for (auto i = range_begin_index; i < number_of_leds; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeInverted)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 3;
	int range_end_index	  = 0;

	for (auto i = range_end_index; i <= range_begin_index; ++i) {
		expected_colors.at(i) = color;
	}

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeBeginEqualsEndFirst)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = 0;

	expected_colors.at(range_begin_index) = color;

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeBeginEqualsEndMiddle)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds / 2 - 1;

	expected_colors.at(range_begin_index) = color;

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeBeginEqualsEndLast)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds - 1;
	int range_end_index	  = number_of_leds - 1;

	expected_colors.at(range_begin_index) = color;

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeBeginEqualsEndLastEqualNumberOfLeds)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds;
	int range_end_index	  = number_of_leds;

	// nothing to do for this test

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeBeginEqualsEndLastHigherThanNumberOfLeds)
{
	RGB color			  = RGB::pure_green;
	int range_begin_index = number_of_leds + 100;
	int range_end_index	  = number_of_leds + 100;

	// nothing to do for this test

	leds.setColorRange(range_begin_index, range_end_index, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}

TEST_F(CoreLedSetColorTest, setColorRangeUpperLimite)
{
	RGB color = RGB::pure_green;

	// nothing to do for this test

	leds.setColorRange(20, 20, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));

	leds.setColorRange(21, 20, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));

	leds.setColorRange(20, 21, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));

	leds.setColorRange(21, 21, color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getColor().begin()));
}
