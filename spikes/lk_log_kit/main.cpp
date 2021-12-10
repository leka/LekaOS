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

[[noreturn]] void log_thread_debug()
{
	while (true) {
		auto start = rtos::Kernel::Clock::now();
		for (auto i = 1; i <= 10; ++i) {
			auto start_run = rtos::Kernel::Clock::now();
			log_debug("Run debug number: %i", i);
			log_debug("%s debug message", "First:");
			log_info("%s information message", "Second:");
			log_error("%s error message", "Third:");
			auto stop_run = rtos::Kernel::Clock::now();
			log_info("Total time to log the %i messages --> %ims\n", 4, int((stop_run - start_run).count()));
		}
		auto stop = rtos::Kernel::Clock::now();

		log_info("Total time to LOG the for loop --> %ims\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(3s);
	}
}

[[noreturn]] void log_thread_printf()
{
	auto log_printf = [](const char *str, auto... params) { printf(str, params...); };

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		for (auto i = 1; i <= 10; ++i) {
			auto start_run = rtos::Kernel::Clock::now();
			log_printf("Run printf number: %i\n", i);
			log_printf("%s debug message\n", "First:");
			log_printf("%s information message\n", "Second:");
			log_printf("%s error message\n", "Third:");
			auto stop_run = rtos::Kernel::Clock::now();
			log_printf("Total time to log the %i messages --> %ims\n\n", 4, int((stop_run - start_run).count()));
		}
		auto stop = rtos::Kernel::Clock::now();

		log_printf("Total time to PRINTF the for loop --> %ims\n\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(3s);
	}
}

auto hello = HelloWorld {};

auto thread_log_debug  = rtos::Thread {};
auto thread_log_printf = rtos::Thread {};

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, Investigation Day!\n");

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	thread_log_debug.start(log_thread_debug);
	rtos::ThisThread::sleep_for(1s);
	thread_log_printf.start(log_thread_printf);

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		auto stop = rtos::Kernel::Clock::now();

		log_info("log_debug took %i ms to complete... that's fast!\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(1000ms);
	}
}
