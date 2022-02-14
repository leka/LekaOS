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

	static constexpr int number_of_leds		= 20;
	static constexpr int default_brightness = 127;

	std::array<bRGB, number_of_leds> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<number_of_leds> leds {spi};
};

TEST_F(CoreLedSetColorTest, setLedsPredefinedForAll)
{
	auto led = bRGB::pure_red;

	std::fill(expected_colors.begin(), expected_colors.end(), led);

	leds.setLeds(led.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsUserDefinedForAll)
{
	auto led = bRGB {210, {120, 12, 56}};

	std::fill(expected_colors.begin(), expected_colors.end(), led);

	leds.setLeds(led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsAtIndexFirst)
{
	auto index = 0;
	auto led   = bRGB::pure_green;

	expected_colors.at(index) = led;

	leds.setLedsAtIndex(index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsAtIndexMiddle)
{
	auto index = number_of_leds / 2 - 1;
	auto led   = bRGB::pure_green;

	expected_colors.at(index) = led;

	leds.setLedsAtIndex(index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsAtIndexLast)
{
	auto index = number_of_leds - 1;
	auto led   = bRGB::pure_green;

	expected_colors.at(index) = led;

	leds.setLedsAtIndex(index, led.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsAtIndexEqualNumberOfLeds)
{
	auto index = number_of_leds;
	auto led   = bRGB::pure_green;

	leds.setLedsAtIndex(index, led.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsAtIndexHigherThanNumberOfLeds)
{
	auto index = number_of_leds + 100;
	auto led   = bRGB::pure_green;

	leds.setLedsAtIndex(index, led.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsAtIndexFirstMiddleEnd)
{
	auto first_index  = 0;
	auto middle_index = number_of_leds / 2 - 1;
	auto end_index	  = number_of_leds - 1;

	auto first_color  = bRGB::pure_red;
	auto middle_color = bRGB::pure_green;
	auto end_color	  = bRGB::pure_blue;

	expected_colors.at(first_index)	 = first_color;
	expected_colors.at(middle_index) = middle_color;
	expected_colors.at(end_index)	 = end_color;

	leds.setLedsAtIndex(first_index, first_color.color);
	leds.setLedsAtIndex(middle_index, middle_color.color);
	leds.setLedsAtIndex(end_index, end_color.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsFromArray)
{
	auto expected_colors =
		std::to_array<bRGB>({bRGB::pure_blue, bRGB::pure_green, bRGB::pure_red,	  bRGB::pure_blue,	bRGB::yellow,
							 bRGB::cyan,	  bRGB::magenta,	bRGB::pure_green, bRGB::pure_red,	bRGB::pure_blue,
							 bRGB::yellow,	  bRGB::cyan,		bRGB::magenta,	  bRGB::pure_green, bRGB::pure_red,
							 bRGB::pure_blue, bRGB::yellow,		bRGB::cyan,		  bRGB::magenta,	bRGB::black});

	leds.setLedsWithArray(expected_colors);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRange)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = 2;
	int range_end_index	  = 5;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeFirstLast)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = number_of_leds - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeFirstMiddle)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = number_of_leds / 2 - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeMiddleLast)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds - 1;

	for (auto i = range_begin_index; i <= range_end_index; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeMiddleLastEqualNumberOfLeds)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds;

	for (auto i = range_begin_index; i < number_of_leds; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeMiddleLastHigherThanNumberOfLeds)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds + 100;

	for (auto i = range_begin_index; i < number_of_leds; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeInverted)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = 3;
	int range_end_index	  = 0;

	for (auto i = range_end_index; i <= range_begin_index; ++i) {
		expected_colors.at(i) = led;
	}

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsBeginEqualsEndFirst)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = 0;
	int range_end_index	  = 0;

	expected_colors.at(range_begin_index) = led;

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsBeginEqualsEndMiddle)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds / 2 - 1;
	int range_end_index	  = number_of_leds / 2 - 1;

	expected_colors.at(range_begin_index) = led;

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsBeginEqualsEndLast)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds - 1;
	int range_end_index	  = number_of_leds - 1;

	expected_colors.at(range_begin_index) = led;

	leds.setLedsRange(range_begin_index, range_end_index, led.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsBeginEqualsEndLastEqualNumberOfLeds)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds;
	int range_end_index	  = number_of_leds;

	// nothing to do for this test

	leds.setLedsRange(range_begin_index, range_end_index, led.color);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsBeginEqualsEndLastHigherThanNumberOfLeds)
{
	auto led			  = bRGB::pure_green;
	int range_begin_index = number_of_leds + 100;
	int range_end_index	  = number_of_leds + 100;

	// nothing to do for this test

	leds.setLedsRange(range_begin_index, range_end_index, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}

TEST_F(CoreLedSetColorTest, setLedsRangeUpperLimite)
{
	auto led = bRGB::pure_green;

	// nothing to do for this test

	leds.setLedsRange(20, 20, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));

	leds.setLedsRange(21, 20, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));

	leds.setLedsRange(20, 21, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));

	leds.setLedsRange(21, 21, led.color, led.brightness);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), leds.getLeds().begin()));
}
