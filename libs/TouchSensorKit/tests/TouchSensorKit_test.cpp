// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "gtest/gtest.h"

using namespace leka;
// using ::testing::_;

class TouchSensorTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	TouchSensorKit touch_sensor_kit = TouchSensorKit();
};

TEST_F(TouchSensorTest, initializationDefault)
{
	ASSERT_NE(&touch_sensor_kit, nullptr);
}

TEST_F(TouchSensorTest, init)
{
	touch_sensor_kit.init();
}

TEST_F(TouchSensorTest, start)
{
	touch_sensor_kit.updateState();
}

TEST_F(TouchSensorTest, run)
{
	touch_sensor_kit.updateState();
}

TEST_F(TouchSensorTest, stop)
{
	touch_sensor_kit.updateState();
}
TEST_F(TouchSensorTest, start)
{
	touch_sensor_kit.updateState();
}

TEST_F(TouchSensorTest, isTouched)
{
	touch_sensor_kit.isTouched(Position::ear_left);
}

TEST_F(TouchSensorTest, isReleased)
{
	touch_sensor_kit.isReleased(Position::ear_left);
}
