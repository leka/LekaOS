// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::NiceMock;

class CoreLEDTests : public ::testing::Test
{
  protected:
	CoreLEDTests() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_belt_leds = 20;
	static constexpr int number_of_ears_leds = 2;
	mock::SPI spimock {};
	CoreLED<number_of_belt_leds> belt {spimock};
	CoreLED<number_of_ears_leds> ears {spimock};

	struct frame {
		static inline auto start = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
		static inline auto reset = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
		static inline auto end	 = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
	};
};

TEST_F(CoreLEDTests, initialisation)
{
	ASSERT_NE(&belt, nullptr);
}

TEST_F(CoreLEDTests, setAllLedsInOneColorAndShowPredefinedColor)
{
	auto color = RGB::pure_red;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColor(color);
	belt.showColor();
}

TEST_F(CoreLEDTests, setAllLedsInOneColorAndShowRandomColor)
{
	auto color = RGB {120, 12, 56};

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColor(color);
	belt.showColor();
}

TEST_F(CoreLEDTests, setOnlyOneLedAndShowColor)
{
	belt.setColor(RGB::black);
	auto color = RGB::pure_green;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(number_of_belt_leds - 1);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColorAtIndex(0, color);
	belt.showColor();
}

TEST_F(CoreLEDTests, setOnlyOneRandomLedAndShowColor)
{
	belt.setColor(RGB::black);
	auto color			  = RGB::pure_green;
	int index_colored_led = 7;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(index_colored_led);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(number_of_belt_leds - index_colored_led - 1);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColorAtIndex(index_colored_led, color);
	belt.showColor();
}

TEST_F(CoreLEDTests, setTwoRandomLedAndShowColor)
{
	belt.setColor(RGB::black);
	auto color1			   = RGB::pure_green;
	auto color2			   = RGB::pure_blue;
	int index_colored_led1 = 7;
	int index_colored_led2 = 17;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(index_colored_led1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color1.blue, color1.green, color1.red))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(index_colored_led2 - index_colored_led1 - 1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color2.blue, color2.green, color2.red))).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(number_of_belt_leds - index_colored_led2 - 1);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColorAtIndex(index_colored_led1, color1);
	belt.setColorAtIndex(index_colored_led2, color2);
	belt.showColor();
}

TEST_F(CoreLEDTests, setAllLedsWithArrayAndShowColor)
{
	std::array<RGB, number_of_belt_leds> expected_color_array = {
		RGB::pure_blue,	 RGB::pure_green, RGB::pure_red,  RGB::pure_blue, RGB::yellow,	RGB::cyan,	  RGB::magenta,
		RGB::pure_green, RGB::pure_red,	  RGB::pure_blue, RGB::yellow,	  RGB::cyan,	RGB::magenta, RGB::pure_green,
		RGB::pure_red,	 RGB::pure_blue,  RGB::yellow,	  RGB::cyan,	  RGB::magenta, RGB::black};

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		for (auto i = 0; i < number_of_belt_leds; i++) {
			EXPECT_CALL(spimock,
						write(testing::ElementsAre(_, expected_color_array.at(i).blue, expected_color_array.at(i).green,
												   expected_color_array.at(i).red)))
				.Times(1);
		}

		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColorWithArray(expected_color_array);
	belt.showColor();
}

TEST_F(CoreLEDTests, setAndShowColorThenHideColor)
{
	EXPECT_CALL(spimock, write(_)).Times(AnyNumber());

	belt.setColor(RGB::pure_green);
	belt.showColor();

	RGB color = RGB::black;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.hideColor();
}

TEST_F(CoreLEDTests, hideColorWhenAlreadyHidden)
{
	belt.hideColor();

	EXPECT_CALL(spimock, write).Times(0);

	belt.hideColor();
}

TEST_F(CoreLEDTests, setAndShowColorWhenColorHidden)
{
	belt.hideColor();

	RGB color = RGB::pure_blue;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.setColor(color);
	belt.showColor();
}

TEST_F(CoreLEDTests, showEarAndBeltColor)
{
	auto color = RGB::pure_blue;

	belt.setColor(color);
	ears.setColor(color);

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_ears_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	belt.showColor();
	ears.showColor();
}
