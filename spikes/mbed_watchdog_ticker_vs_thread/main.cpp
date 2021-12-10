// Leka - LekaOS
// Copyright (c) 2020 Arm Limited and affiliates.
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"
#include "drivers/Ticker.h"
#include "drivers/Watchdog.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

///
/// @brief Uncomment the line you want to test. `USE_THREAD` will run the example with
/// a thread, whereas `USE_TICKER` will run the example with a ticker.
///

#define USE_THREAD
// #define USE_TICKER

//
// MARK:- Variables
//

struct variables {
	static inline auto led_main		= mbed::DigitalOut {LED1};
	static inline auto led_error	= mbed::DigitalOut {LED3};
	static inline auto led_watchdog = mbed::DigitalOut {LED2};

	static inline auto button = mbed::InterruptIn {USER_BUTTON};

	static inline auto &watchdog = mbed::Watchdog::get_instance();

	static inline auto thread_watchdog = rtos::Thread {};
	static inline auto ticker_watchdog = mbed::Ticker {};

	static inline auto should_abort		= false;
	static inline auto should_get_stuck = false;
};

//
// MARK:- Functions
//

void kickWatchdog()
{
	variables::watchdog.kick();
	variables::led_watchdog = !variables::led_watchdog;
}

// Abort or run infinite loop
void buttonInterrupt()
{
	// Comment the line you don't need
	// variables::should_abort = true;
	variables::should_get_stuck = true;
}

//
// MARK:- Thread
//

#ifdef USE_THREAD

void watchdogThreadFunction()
{
	while (true) {
		kickWatchdog();
		rtos::ThisThread::sleep_for(513ms);
	}
}

void initWatchdog()
{
	variables::watchdog.start(5000);
	variables::thread_watchdog.start(watchdogThreadFunction);
	variables::thread_watchdog.set_priority(osPriorityLow);
}

#endif

//
// MARK:- Ticker
//

#ifdef USE_TICKER

void initWatchdog()
{
	variables::watchdog.start(5000);
	variables::ticker_watchdog.attach(kickWatchdog, 500ms);
}

#endif

//
// MARK:- Main thread
//

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	variables::led_error = 0;

	variables::led_main		= 1;
	variables::led_watchdog = 1;

	rtos::ThisThread::sleep_for(1s);

	variables::led_main		= 0;
	variables::led_watchdog = 0;

	rtos::ThisThread::sleep_for(1s);

	variables::led_main		= 1;
	variables::led_watchdog = 1;

	rtos::ThisThread::sleep_for(1s);

	variables::led_main		= 0;
	variables::led_watchdog = 0;

	rtos::ThisThread::sleep_for(1s);

	initWatchdog();

	variables::button.rise(&buttonInterrupt);

	while (true) {
		variables::led_main = !variables::led_main;

		rtos::ThisThread::sleep_for(100ms);

		if (variables::should_abort) {
			variables::led_error = !variables::led_error;
			abort();
		}

		if (variables::should_get_stuck) {
			while (true) {
				variables::led_error = !variables::led_error;
				// Comment the type of delay you want to test
				HAL_Delay(200);
				// rtos::ThisThread::sleep_for(200ms);
			}
		}
	}
}
