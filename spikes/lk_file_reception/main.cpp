// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FileManager.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SerialUtils.h"

using namespace leka;
using namespace std::chrono;

FIL file;
FileManager sd_card;

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	f_open(&file, "update.bin", FA_READ | FA_WRITE | FA_CREATE_ALWAYS);

	auto application_size = receptionBySerial(serial, file);

	f_close(&file);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(1s);

		log_info("Expected size: %ld | Actual size: %ld\n", application_size, f_size(&file));
	}
}
