// mbed Microcontroller Library
// Copyright (c) 2019 ARM Limited
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/UnbufferedSerial.h"
#include "rtos/ThisThread.h"

using namespace std::chrono;

auto serial = mbed::UnbufferedSerial {USBTX, USBRX, 115200};

auto main() -> int
{
	// Initialise the digital pin LED1 as an output
	mbed::DigitalOut led(LED1);

	while (true) {
		led = !led;
		serial.write(".\n", 2);
		rtos::ThisThread::sleep_for(10s);
		led = !led;
		serial.write(".\n", 2);
		rtos::ThisThread::sleep_for(10s);
	}
}
