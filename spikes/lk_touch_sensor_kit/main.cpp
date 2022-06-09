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
	touch_sensor_kit.adjust_sensitivity_high();
	rtos::ThisThread::sleep_for(2s);

	// auto memory			  = std::array<uint8_t, 24> {};
	// touch_sensor_kit.read_dac_memory(memory);
	// for (unsigned char i: memory) {
	// 	log_info("%02x", i);
	// }
	// log_info("\n\n");

	while (true) {
		touch_sensor_kit.updateState();
		touch_sensor_kit.printState();
		log_info("\n\n");

		rtos::ThisThread::sleep_for(1s);
	}
}
