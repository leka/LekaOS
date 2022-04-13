// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "CoreRFIDReader.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "RFIDKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	auto core_buffered_serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto core_rfid_reader	  = CoreRFIDReader(core_buffered_serial);
	auto rfid_kit			  = RFIDKit(core_rfid_reader);

	rtos::ThisThread::sleep_for(2s);

	rfid_kit.init();

	HelloWorld hello;
	hello.start();

	while (true) {
		rtos::ThisThread::sleep_for(10ms);
	}
}
