// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "HelloWorld.h"
#include "LekaBluetooth.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	Bluetooth bluetooth;
	rtos::Thread bluetooth_thread;
	bluetooth_thread.start({&bluetooth, &Bluetooth::start});

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(1s);
	}
}
