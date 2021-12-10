// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "LekaTouch.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	Touch touch_sensor;
	rtos::Thread touch_thread;
	touch_thread.start({&touch_sensor, &Touch::start});

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);
	}
}
