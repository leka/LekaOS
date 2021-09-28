// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace leka;
using namespace mbed;
using namespace std::chrono;

auto main() -> int
{
	static auto serial = BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	rtos::ThisThread::sleep_for(1s);

	while (true) {
		auto start = rtos::Kernel::Clock::now();

		log_debug("Message from %s --> \"%s\" at %i ms", MBED_CONF_APP_TARGET_NAME, "0123456789",
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));

		auto stop = rtos::Kernel::Clock::now();

		log_info("log_debug took %i ms\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(1000ms);
	}
}
