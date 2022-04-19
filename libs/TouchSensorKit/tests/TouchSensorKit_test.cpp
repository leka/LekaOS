// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "IOKit/DigitalIn.h"
#include "gtest/gtest.h"

using namespace leka;
// using ::testing::_;

class TouchSensorTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}
	mbed::I2C i2c	= mbed::I2C(PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL);
	CoreI2C corei2c = CoreI2C {i2c};
	CoreIOExpanderMCP23017 expander			= CoreIOExpanderMCP23017 {corei2c};
	io::expanded::DigitalIn<uint16_t> input = io::expanded::DigitalIn<uint16_t> {expander, touch::pin::ear_left};
	TouchSensorKit touch_sensor_kit			= TouchSensorKit {input};
};

TEST_F(TouchSensorTest, initializationDefault)
{
	ASSERT_NE(&touch_sensor_kit, nullptr);
}

TEST_F(TouchSensorTest, isTouched)
{
	auto touched = touch_sensor_kit.isTouched();
	ASSERT_TRUE(touched);
}
