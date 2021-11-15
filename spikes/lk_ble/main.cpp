// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "BLEGap.h"

#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!");

	auto &ble	 = BLE::Instance();
	auto ble_gap = BLEGap {ble};

	ble_gap.start();

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
