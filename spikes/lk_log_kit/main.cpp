// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace mbed;
using namespace std::chrono;

[[noreturn]] void log_thread()
{
	while (true) {
		auto start = rtos::Kernel::Clock::now();
		log_debug("%s debug message", "First:");
		log_info("%s information message", "Second:");
		log_error("%s error message", "Third:");
		auto stop = rtos::Kernel::Clock::now();
		log_info("Total time to log the %i message --> %ims\n", 3, int((stop - start).count()));
		rtos::ThisThread::sleep_for(4000ms);
	}
}

auto main() -> int
{
	static auto serial = BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_trace_function([](const char *str, size_t size) { serial.write(str, size); });

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, Investigation Day!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	auto thread_log = rtos::Thread();
	thread_log.start(log_thread);

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		auto stop = rtos::Kernel::Clock::now();

		log_info("log_debug took %i ms to complete... that's fast!\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(1000ms);
	}
}
