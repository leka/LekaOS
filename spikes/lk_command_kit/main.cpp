// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;

auto main() -> int
{
	logger::init();

	log_info("Hello, spike CommandKit!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
	}
}
