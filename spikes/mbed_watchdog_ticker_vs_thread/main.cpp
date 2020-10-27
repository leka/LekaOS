// Leka - LekaOS
// Copyright (c) 2020 Arm Limited and affiliates.
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

///
/// @brief Uncomment the line you want to test. `USE_THREAD` will run the example with
/// a thread, whereas `USE_TICKER` will run the example with a ticker.
///

#define USE_THREAD
// #define USE_TICKER

//
// MARK:- Variables
//

DigitalOut led_main(LED1);
DigitalOut led_error(LED3);
DigitalOut led_watchdog(LED2);

InterruptIn button(USER_BUTTON);

Watchdog &watchdog = Watchdog::get_instance();

bool should_abort	  = false;
bool should_get_stuck = false;

//
// MARK:- Functions
//

void kickWatchdog()
{
	watchdog.kick();
	led_watchdog = !led_watchdog;
}

///
/// @brief Abort or run infinite loop
///
/// Comment the line you don't need
///
void buttonInterrupt()
{
	// should_abort = true;
	should_get_stuck = true;
}

//
// MARK:- Thread
//

#ifdef USE_THREAD

Thread thread_watchdog;

void watchdogThreadFunction()
{
	while (true) {
		kickWatchdog();
		rtos::ThisThread::sleep_for(513ms);
	}
}

void initWatchdog()
{
	watchdog.start(5000);
	thread_watchdog.start(watchdogThreadFunction);
	thread_watchdog.set_priority(osPriorityLow);
}

#endif

//
// MARK:- Ticker
//

#ifdef USE_TICKER

Ticker ticker_watchdog;

void initWatchdog()
{
	watchdog.start(5000);
	ticker_watchdog.attach(kickWatchdog, 500ms);
}

#endif

//
// MARK:- Main thread
//

int main()
{
	printf("Hello\n");

	led_error = 0;

	led_main	 = 1;
	led_watchdog = 1;

	rtos::ThisThread::sleep_for(1s);

	led_main	 = 0;
	led_watchdog = 0;

	rtos::ThisThread::sleep_for(1s);

	led_main	 = 1;
	led_watchdog = 1;

	rtos::ThisThread::sleep_for(1s);

	led_main	 = 0;
	led_watchdog = 0;

	rtos::ThisThread::sleep_for(1s);

	initWatchdog();

	button.rise(&buttonInterrupt);

	while (1) {
		led_main = !led_main;

		rtos::ThisThread::sleep_for(100ms);

		if (should_abort) {
			led_error = !led_error;
			abort();
		}

		if (should_get_stuck) {
			while (true) {
				led_error = !led_error;
				// Comment the type of delay you want to test
				HAL_Delay(200);
				// rtos::ThisThread::sleep_for(200ms);
			}
		}
	}
}
