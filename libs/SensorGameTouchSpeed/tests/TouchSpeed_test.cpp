// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSpeed.h"

#include "gtest/gtest.h"

using namespace leka;
// using ::testing::_;
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
