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
	auto color = RGB::pure_red;

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.setColor(color);
	belt.show();
}

TEST_F(CoreLedShowHideColorTest, showUserDefinedColor)
{
	auto color = RGB {120, 12, 56};

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.setColor(color);
	belt.show();
}

TEST_F(CoreLedShowHideColorTest, hideColorAfterInit)
{
	EXPECT_CALL(belt_spimock, write).Times(0);

	belt.hide();
}

TEST_F(CoreLedShowHideColorTest, hideColorAfterShow)
{
	EXPECT_CALL(belt_spimock, write(_)).Times(AnyNumber());

	belt.setColor(RGB::pure_green);
	belt.show();

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock, write(testing::ElementsAre(_, RGB::black.blue, RGB::black.green, RGB::black.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.hide();
}

TEST_F(CoreLedShowHideColorTest, hideColorWhenAlreadyHidden)
{
	belt.hide();

	EXPECT_CALL(belt_spimock, write).Times(0);

	belt.hide();
}

TEST_F(CoreLedShowHideColorTest, showColorAfterHideAndSet)
{
	belt.hide();

	RGB color = RGB::pure_blue;
	belt.setColor(color);

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);
	}

	belt.show();
}

TEST_F(CoreLedShowHideColorTest, showEarAndBeltColor)
{
	auto color = RGB::white;

	belt.setColor(color);
	ears.setColor(color);

	{
		InSequence seq;

		EXPECT_CALL(belt_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(belt_spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_belt_leds);
		EXPECT_CALL(belt_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(belt_spimock, write(frame::end)).Times(1);

		EXPECT_CALL(ears_spimock, write(frame::start)).Times(1);
		EXPECT_CALL(ears_spimock, write(testing::ElementsAre(_, color.blue, color.green, color.red)))
			.Times(number_of_ears_leds);
		EXPECT_CALL(ears_spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(ears_spimock, write(frame::end)).Times(1);
	}

	belt.show();
	ears.show();
}
