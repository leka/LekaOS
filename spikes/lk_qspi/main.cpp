// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "HelloWorld.h"
#include "LekaFirmware.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::Thread qspi_thread;
	Firmware qspi;

	qspi_thread.start({&qspi, &Firmware::start});

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world, int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);
	}
}
