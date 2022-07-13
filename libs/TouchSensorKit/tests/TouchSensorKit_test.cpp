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

TEST_F(TouchSensorTest, updateState)
{
	touch_sensor_kit.updateState();
}

TEST_F(TouchSensorTest, printState)
{
	touch_sensor_kit.printState();
}

TEST_F(TouchSensorTest, resetByPowerMode)
{
	touch_sensor_kit.resetByPowerMode();
}

TEST_F(TouchSensorTest, calibration)
{
	touch_sensor_kit.caliration();
}
