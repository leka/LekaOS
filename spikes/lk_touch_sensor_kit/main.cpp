// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	auto i2c			  = mbed::I2C(PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL);
	auto corei2c		  = CoreI2C {i2c};
	auto expander		  = CoreIOExpanderMCP23017 {corei2c};
	auto ear_left_input	  = io::expanded::DigitalIn<uint16_t> {expander, touch::pin::ear_left};
	auto touch_sensor_kit = TouchSensorKit {ear_left_input};

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	auto t = rtos::Kernel::Clock::now() - start;
	log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
			 int(t.count() / 1000));

	while (true) {
		expander.setModeForPin(touch::pin::ear_left, PinMode::PullNone);

		if (auto mode = expander.getModeForPin(touch::pin::ear_left); mode == PinMode::PullUp) {
			log_info("Pull Up");
		} else {
			log_info("Not Pull Up");
		}

		expander.setModeForPin(touch::pin::ear_left, PinMode::PullUp);

		if (auto mode = expander.getModeForPin(touch::pin::ear_left); mode == PinMode::PullUp) {
			log_info("Pull Up");
		} else {
			log_info("Not Pull Up");
		}

		if (auto touched = touch_sensor_kit.isTouched(); touched) {
			log_info("Touched !\n\n");
		} else {
			log_info(" Not Touched !\n\n");
		}

		rtos::ThisThread::sleep_for(5s);
	}
}
