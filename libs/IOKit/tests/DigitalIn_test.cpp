// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "../include/DigitalIn.h"

#include "gtest/gtest.h"
#include "mocks/leka/IOExpander.h"

using namespace leka;
using ::testing::_;

class DigitalInTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	leka::mock::IOExpander mockIOExpander {};
	uint16_t pin_number = 0;
};

TEST_F(DigitalInTest, instantiation)
{
	EXPECT_CALL(mockIOExpander, setInputPins(pin_number)).Times(1);

	leka::io::expanded::DigitalIn pin {mockIOExpander, pin_number};

	ASSERT_NE(&pin, nullptr);
}
