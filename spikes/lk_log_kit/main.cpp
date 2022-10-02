// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "drivers/Ticker.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreEventQueue.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "include/utils.h"

using namespace leka;
using namespace mbed;
using namespace std::chrono;

auto hello = HelloWorld {};

auto thread_log_debug  = rtos::Thread {};
auto thread_log_printf = rtos::Thread {};

auto ticker_log_from_isr = Ticker {};
auto event_queue		 = CoreEventQueue {};

auto main() -> int
{
	logger::init();
	event_queue.dispatch_forever();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, Investigation Day!\n");

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	// ? You can un/comment the following lines to filter console output
	// thread_log_debug.start(log_thread_debug);
	// rtos::ThisThread::sleep_for(1s);
	// thread_log_printf.start(log_thread_printf);

	auto log_isr_lambda = [] {
		// ? After testing, it turns out that LogKit cannot be easily used from ISR
		// ? This is because it uses snprintf to format the output which is not ISR compatible
		// ? One can test the faulty behavior here by un/commenting the corresponding line

		// ? w/ event queue
		event_queue.call(&log_from_isr);
		// ? w/o event queue
		// log_from_isr();
	};

	ticker_log_from_isr.attach(log_isr_lambda, 2s);

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		auto stop = rtos::Kernel::Clock::now();

		log_info("log_debug took %i ms to complete... that's fast!\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(1000ms);
	}
}
