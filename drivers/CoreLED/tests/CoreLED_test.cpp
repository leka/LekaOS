// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;

class CoreLEDTests : public ::testing::Test
{
  protected:
	CoreLEDTests() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	static constexpr int number_of_leds = 20;
	mock::SPI spimock {};
	CoreLED coreled {spimock, number_of_leds};

	struct frame {
		static inline auto start = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
		static inline auto reset = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
		static inline auto end	 = testing::ElementsAre(0x00, 0x00, 0x00, 0x00);
	};
};

TEST_F(CoreLEDTests, initialisation)
{
	ASSERT_NE(&coreled, nullptr);
}

TEST_F(CoreLEDTests, setColorAndShowPredefinedColor)
{
	auto color = RGB::pure_red;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.red, color.green, color.blue))).Times(number_of_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	coreled.setColor(color);
	coreled.showColor();
}

TEST_F(CoreLEDTests, setColorAndShowRandomColor)
{
	auto color = RGB {120, 12, 56};

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.red, color.green, color.blue))).Times(number_of_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	coreled.setColor(color);
	coreled.showColor();
}

TEST_F(CoreLEDTests, setAndShowColorThenHideColor)
{
	coreled.setColor(RGB::pure_green);
	coreled.showColor();

	RGB color = RGB::black;

	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.red, color.green, color.blue))).Times(number_of_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	coreled.hideColor();
}

TEST_F(CoreLEDTests, hideColorWhenAlreadyHidden)
{
	coreled.hideColor();

	EXPECT_CALL(spimock, write).Times(0);

	coreled.hideColor();
}

TEST_F(CoreLEDTests, setAndShowColorWhenColorHidden)
{
	coreled.hideColor();

	RGB color = RGB::pure_blue;
	{
		InSequence seq;

		EXPECT_CALL(spimock, write(frame::start)).Times(1);
		EXPECT_CALL(spimock, write(testing::ElementsAre(_, color.red, color.green, color.blue))).Times(number_of_leds);
		EXPECT_CALL(spimock, write(frame::reset)).Times(1);
		EXPECT_CALL(spimock, write(frame::end)).Times(1);
	}

	coreled.setColor(color);
	coreled.showColor();
}

TEST_F(CoreLEDTests, setAndShowBlackThenCheckIfIsOff)
{
	coreled.setColor(RGB::black);
	coreled.showColor();

	EXPECT_FALSE(coreled.isOn());
}

TEST_F(CoreLEDTests, setAndShowBlackThenHide)
{
	coreled.setColor(RGB::black);
	coreled.showColor();

	EXPECT_FALSE(coreled.isOn());

	coreled.hideColor();

	EXPECT_CALL(spimock, write).Times(0);
	EXPECT_FALSE(coreled.isOn());
}
