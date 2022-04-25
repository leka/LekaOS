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

	auto rfidserial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto rfidreader = CoreRFIDReader(rfidserial);
	auto rfidkit	= RFIDKit(rfidreader);

	rtos::ThisThread::sleep_for(2s);

	rfidkit.init();

	HelloWorld hello;
	hello.start();

	while (true) {
		rtos::ThisThread::sleep_for(10s);
	}
}
