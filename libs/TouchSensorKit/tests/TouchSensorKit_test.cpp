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

TEST_F(TouchSensorTest, reset)
{
	touch_sensor_kit.reset();
}

TEST_F(TouchSensorTest, pullUp)
{
	touch_sensor_kit.pull_up();
}

TEST_F(TouchSensorTest, setPowerModeNormal)
{
	touch_sensor_kit.set_power_mode(touch::power_mode::normal);
}

TEST_F(TouchSensorTest, updateState)
{
	touch_sensor_kit.updateState();
}

TEST_F(TouchSensorTest, printState)
{
	touch_sensor_kit.printState();
}
