// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include "CoreTouchSensor.h"

// #include "CoreDACTouch.h"
// #include "CoreIOExpander.h"
// #include "DigitalOut.h"
// #include "IOKit/DigitalIn.h"
// #include "IOKit/DigitalOut.h"
// #include "gmock/gmock.h"
// #include "gtest/gtest.h"
// #include "mocks/leka/CoreI2C.h"

// using namespace leka;

// using ::testing::Args;
// using ::testing::DoAll;
// using ::testing::ElementsAre;
// using ::testing::Return;
// using ::testing::SetArrayArgument;

// class CoreTouchSensorTest : public ::testing::Test
// {
//   protected:
// 	void SetUp() override {}
// 	void TearDown() override {}

// 	const uint8_t i2c_address {0xC0};
// 	mock::CoreI2C mocki2c;
// 	dac {mocki2c, i2c_address};
// 	uint8_t channel {1};

// 	mbed::DigitalOut io_expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
// 	CoreIOExpanderMCP23017 io_expander {mocki2c, io_expander_reset};
// 	uint8_t pin_number {0x0001};

// 	leka::io::expanded::DigitalIn<> input {io_expander, pin_number};
// 	leka::io::expanded::DigitalOut<> power_mode {io_expander, pin_number};

// 	CoreTouchSensor sensor {input, power_mode, dac, channel};
// };

// TEST_F(CoreTouchSensorTest, initializationDefault)
// {
// 	auto new_sensor = CoreTouchSensor {input, power_mode, dac, channel};
// 	ASSERT_NE(&new_sensor, nullptr);
// }

// TEST_F(CoreTouchSensorTest, read)
// {
// 	sensor.read();
// }

// TEST_F(CoreTouchSensorTest, setPowerMode)
// {
// 	sensor.setPowerMode(0x01);
// }

// TEST_F(CoreTouchSensorTest, adjustSensitivity)
// {
// 	auto value = 0x0ABC;
// 	sensor.adjust_sensitivity(channel, value);
// }
