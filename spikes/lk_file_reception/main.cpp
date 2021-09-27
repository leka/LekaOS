// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "FileReception.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(10s);
	}
}
