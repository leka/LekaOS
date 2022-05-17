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

TEST_F(TouchSensorTest, setup)
{
	touch_sensor_kit.setup();
}

TEST_F(TouchSensorTest, setPullModePullUp)
{
	touch_sensor_kit.set_pull_mode(PinMode::PullUp);
}

TEST_F(TouchSensorTest, setPullModePullDown)
{
	touch_sensor_kit.set_pull_mode(PinMode::PullDown);
}

TEST_F(TouchSensorTest, setPowerModeNormal)
{
	touch_sensor_kit.set_power_mode(touch::power_mode::normal);
}

TEST_F(TouchSensorTest, setPowerModeLow)
{
	touch_sensor_kit.set_power_mode(touch::power_mode::low);
}

TEST_F(TouchSensorTest, resetByPowerMode)
{
	touch_sensor_kit.power_mode_reset();
}

TEST_F(TouchSensorTest, updateState)
{
	touch_sensor_kit.updateState();
}

TEST_F(TouchSensorTest, printState)
{
	touch_sensor_kit.printState();
}
