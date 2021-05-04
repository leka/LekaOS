// mbed Microcontroller Library
// Copyright (c) 2019 ARM Limited
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"

using namespace std::chrono;

auto main() -> int
{
	// Initialise the digital pin LED1 as an output
	mbed::DigitalOut led(LED1);

	while (true) {
		led = !led;
		rtos::ThisThread::sleep_for(500ms);
		led = !led;
		rtos::ThisThread::sleep_for(100ms);
	}
}
