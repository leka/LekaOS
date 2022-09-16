// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreMCU.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SerialNumberKit.h"

using namespace leka;
using namespace std::chrono_literals;

auto mcu			 = CoreMCU {};
auto serialnumberkit = SerialNumberKit {mcu};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	rtos::ThisThread::sleep_for(1s);

	auto mcu_id = mcu.getID();
	log_info("MCU ID: %08lx %08lx %08lx", mcu_id.front, mcu_id.middle, mcu_id.back);

	auto serial_number = serialnumberkit.getSerialNumber();
	log_info("S/N: %s", serial_number.data());

	auto short_serial_number = serialnumberkit.getShortSerialNumber();
	log_info("S/N (short): %s", short_serial_number.data());

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(10s);
	}
}
