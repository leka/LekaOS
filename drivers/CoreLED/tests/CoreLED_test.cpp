// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

#include "gtest/gtest.h"

using namespace leka;

class CoreLEDTests : public ::testing::Test
{
  protected:
    CoreLEDTests() : corespi(spi), coreled(corespi) {}

    void SetUp() override
    {
        expected_belt_color = LEDColor::pure_green;
    }
    // void TearDown() override {}

	CoreLED coreled;
	mbed::SPI spi {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK, NC};
	CoreSPI corespi;

	LEDColor expected_belt_color;
	LEDColor expected_belt_color_when_off;
};

TEST_F(CoreLEDTests, initialisation)
{
    ASSERT_NE(&coreled, nullptr);
}

TEST_F(CoreLEDTests, turnBeltOff)
{
	coreled.turnOff();

    ASSERT_EQ(false, coreled.getState());
	ASSERT_EQ(false, coreled.getState());

}

TEST_F(CoreLEDTests, turnBeltOn)
{
	coreled.turnOn();

    ASSERT_EQ(true, coreled.getState());
}

TEST_F(CoreLEDTests, setImplementedBeltColor)
{
	coreled.setBeltColor(expected_belt_color);
    ASSERT_EQ(expected_belt_color, coreled.getBeltColor());
}

TEST_F(CoreLEDTests, setUnimplementedBeltColor)
{
	expected_belt_color = LEDColor {255,215,0};
	coreled.setBeltColor(expected_belt_color);
    ASSERT_EQ(expected_belt_color, coreled.getBeltColor());
}





