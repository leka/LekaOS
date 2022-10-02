// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"

#include "CoreTicker.h"
#include "CoreTimeout.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto ticker	 = CoreTicker {};
auto timeout = CoreTimeout {};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	mbed::DigitalOut led {LED1, 0};

	auto lambda = [&led]() { led = !led; };

	log_info("Use of ticker");

	ticker.onTick(lambda);

	ticker.start(1s);
	rtos::ThisThread::sleep_for(5s);   // lambda is called 5 times
	ticker.stop();

	ticker.start(1s);
	rtos::ThisThread::sleep_for(500ms);	  // lambda is not called
	ticker.stop();

	log_info("Use of timeout");

	timeout.onTimeout(lambda);

	timeout.start(1s);
	rtos::ThisThread::sleep_for(5s);   // lambda is called 1 time
	ticker.stop();					   // optional

	timeout.start(1s);
	rtos::ThisThread::sleep_for(500ms);	  // lambda is not called
	timeout.stop();

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
