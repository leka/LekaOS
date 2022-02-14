// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"
#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;
using ::testing::InSequence;

class CoreLedShowHideColorTest : public ::testing::Test
{
  protected:
	CoreLedShowHideColorTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_belt_leds = 20;
	static constexpr int number_of_ears_leds = 2;

	mock::SPI belt_spimock {};
	mock::SPI ears_spimock {};

	CoreLED<number_of_belt_leds> belt {belt_spimock};
	CoreLED<number_of_ears_leds> ears {ears_spimock};

	struct frame {
		static inline auto start = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
		static inline auto reset = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
		static inline auto end	 = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
	};
};

TEST_F(CoreLedShowHideColorTest, initialisation)
{
	ASSERT_NE(&belt, nullptr);
}

TEST_F(CoreLedShowHideColorTest, showPredefinedColor)
{
	auto led = bRGB::pure_red;

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock,
					write(testing::ElementsAre(led.brightness, led.color.blue, led.color.green, led.color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.setLeds(led.color);
	belt.showLeds();
}

TEST_F(CoreLedShowHideColorTest, showUserDefinedColor)
{
	auto led = bRGB {210, {120, 12, 56}};

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock,
					write(testing::ElementsAre(led.brightness, led.color.blue, led.color.green, led.color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.setLeds(led.color, led.brightness);
	belt.showLeds();
}

TEST_F(CoreLedShowHideColorTest, hideColorAfterInit)
{
	EXPECT_CALL(belt_spimock, write).Times(0);

	belt.hideColor();
}

TEST_F(CoreLedShowHideColorTest, hideColorAfterShow)
{
	belt.setLeds(RGB::pure_green);
	belt.showLeds();

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock,
					write(testing::ElementsAre(0, RGB::pure_green.blue, RGB::pure_green.green, RGB::pure_green.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.hideColor();
}

TEST_F(CoreLedShowHideColorTest, hideColorWhenAlreadyHidden)
{
	belt.hideColor();

	EXPECT_CALL(belt_spimock, write).Times(0);

	belt.hideColor();
}

TEST_F(CoreLedShowHideColorTest, showLedsAfterHideAndSet)
{
	belt.hideColor();

	bRGB led = bRGB::pure_blue;
	belt.setLeds(led.color, led.brightness);

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock,
					write(testing::ElementsAre(led.brightness, led.color.blue, led.color.green, led.color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.showLeds();
}

TEST_F(CoreLedShowHideColorTest, showEarAndBeltColor)
{
	auto led = bRGB::white;

	belt.setLeds(led.color, led.brightness);
	ears.setLeds(led.color, led.brightness);

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock,
					write(testing::ElementsAre(led.brightness, led.color.blue, led.color.green, led.color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);

		EXPECT_CALL(ears_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(ears_spimock,
					write(testing::ElementsAre(led.brightness, led.color.blue, led.color.green, led.color.red)))
			.Times(number_of_ears_leds);
		EXPECT_CALL(ears_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(ears_spimock, write(frame::end)).Times(1);
	}

	belt.showLeds();
	ears.showLeds();
}
