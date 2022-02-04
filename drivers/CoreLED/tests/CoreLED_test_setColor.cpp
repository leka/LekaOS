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

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<number_of_leds> leds {spi};
};

TEST_F(CoreLedSetColorTest, setColorPredefinedForAll)
{
	auto expected_color = RGB::pure_red;

	leds.setColor(expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorUserDefinedForAll)
{
	auto expected_color = RGB {120, 12, 56};

	leds.setColor(expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorAtIndexFirst)
{
	auto index			= 0;
	auto expected_color = RGB::pure_green;

	leds.setColorAtIndex(index, expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorAtIndexMiddle)
{
	auto index			= number_of_leds / 2;
	auto expected_color = RGB::pure_green;

	leds.setColorAtIndex(index, expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorAtIndexLast)
{
	auto index			= number_of_leds - 1;
	auto expected_color = RGB::pure_green;

	leds.setColorAtIndex(index, expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorAtIndexEqualNumberOfLeds)
{
	auto index			= number_of_leds;
	auto expected_color = RGB::pure_green;

	leds.setColorAtIndex(index, expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorAtIndexHigherThanNumberOfLeds)
{
	auto index			= number_of_leds + 1;
	auto expected_color = RGB::pure_green;

	leds.setColorAtIndex(index, expected_color);

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorAtIndexFirstMiddleEnd)
{
	auto first_index  = 0;
	auto middle_index = number_of_leds / 2;
	auto end_index	  = number_of_leds - 1;

	auto first_color  = RGB::pure_red;
	auto middle_color = RGB::pure_green;
	auto end_color	  = RGB::pure_blue;

	leds.setColorAtIndex(first_index, first_color);
	leds.setColorAtIndex(middle_index, middle_color);
	leds.setColorAtIndex(end_index, end_color);	  // TODO catch error

	// TODO
}

TEST_F(CoreLedSetColorTest, setColorFromArray)
{
	auto expected_colors = std::to_array<RGB>(
		{RGB::pure_blue,  RGB::pure_green, RGB::pure_red,  RGB::pure_blue, RGB::yellow,	 RGB::cyan,	   RGB::magenta,
		 RGB::pure_green, RGB::pure_red,   RGB::pure_blue, RGB::yellow,	   RGB::cyan,	 RGB::magenta, RGB::pure_green,
		 RGB::pure_red,	  RGB::pure_blue,  RGB::yellow,	   RGB::cyan,	   RGB::magenta, RGB::black});

	leds.setColorWithArray(expected_colors);

	// TODP
}

TEST_F(CoreLedSetColorTest, setRangeOfLedsAndShowColor)
{
	RGB color		= RGB::pure_green;
	int range_start = 4;
	int range_end	= 15;

	leds.setColorRange(range_start, range_end, color);

	// TODO
}
