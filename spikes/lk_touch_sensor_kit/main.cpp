// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

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

	auto touch_sensor_kit = TouchSensorKit();
	touch_sensor_kit.setup();

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	auto t = rtos::Kernel::Clock::now() - start;
	log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
			 int(t.count() / 1000));
	touch_sensor_kit.adjustSensitivity(0x0000, true);
	rtos::ThisThread::sleep_for(2s);

	while (true) {
		touch_sensor_kit.updateState();
		touch_sensor_kit.printState();
		touch_sensor_kit.resetByPowerMode();
		log_info("\n\n");
		rtos::ThisThread::sleep_for(1s);
	}
}
