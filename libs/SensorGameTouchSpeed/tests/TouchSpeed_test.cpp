// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSpeed.h"

#include "gtest/gtest.h"

using namespace leka;
// using ::testing::_;

auto random8(uint8_t min, uint8_t max) -> uint8_t
{
	return min + rand() % (max - min + 1);
}
class TouchSpeedTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	TouchSpeed touch_speed = TouchSpeed();
};

TEST_F(TouchSpeedTest, helloWorldTest)
{
	touch_speed.HelloWorld();
}
TEST_F(TouchSpeedTest, MaximumDurationTest)
{
	touch_speed.determineMaximumDuration(1);
	ASSERT_EQ(touch_speed.determineMaximumDuration(1), 5000);
}
