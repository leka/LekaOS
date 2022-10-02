// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreQSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n");

	auto coreqspi = CoreQSPI();

	auto coremanageris25lp = CoreFlashManagerIS25LP016D(coreqspi);
	auto coreis25lp		   = CoreFlashIS25LP016D(coreqspi, coremanageris25lp);
	coreis25lp.reset();

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	coreqspi.setDataTransmissionFormat();
	coreqspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);

	const size_t bytes_to_read = 0x20;
	std::array<uint8_t, bytes_to_read> buffer {};

	auto data		 = std::to_array<uint8_t>({1, 2, 3, 4, 5, 6, 7, 8, 9});
	uint32_t address = 0x00;

	coreis25lp.erase();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(200ms);

		auto bytes_written = coreis25lp.write(address, data, std::size(data));

		auto bytes_read = coreis25lp.read(address, buffer, bytes_written);
		// ? Use debug to spy values

		if (auto new_address = address + bytes_to_read; new_address > coreis25lp.getSize()) {
			address = 0x00;
		} else {
			address = new_address;
		}
	}
}
