// mbed Microcontroller Library
// Copyright (c) 2019 ARM Limited
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/Ticker.h"
#include "drivers/UnbufferedSerial.h"
#include "rtos/ThisThread.h"

using namespace std::chrono;

static inline auto serial = mbed::UnbufferedSerial {USBTX, USBRX, 115200};

static inline auto ticker = mbed::Ticker {};

void tick()
{
	serial.write("\n", 1);
}

auto main() -> int
{
	// Initialise the digital pin LED1 as an output
	mbed::DigitalOut led(LED1);
	ticker.attach(tick, 10s);

	while (true) {
		rtos::ThisThread::sleep_for(333s);
		// // led = !led;
		// serial.write("\n", 1);
		// HAL_Delay(10000);
		// // led = !led;
		// serial.write("\n", 1);
		// HAL_Delay(10000);
	}
}
