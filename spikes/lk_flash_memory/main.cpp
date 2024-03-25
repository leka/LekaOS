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

	const std::size_t bytes_to_read = 0x100;
	std::array<uint8_t, bytes_to_read> buffer {};

	auto data		 = std::to_array<uint8_t>({1, 2, 3, 4, 5, 6, 7, 8, 9});
	uint32_t address = 0x00;

	// coreis25lp.erase();

	auto status = coremanageris25lp.getStatusRegister();
	log_info("Status: %x", status);

	rtos::ThisThread::sleep_for(1s);
	auto start = rtos::Kernel::Clock::now();

	while (true) {
		// auto bytes_written = coreis25lp.write(address, data, std::size(data));

		auto bytes_read = coreis25lp.read(address, buffer, buffer.size());
		// ? Use debug to spy values

		{
			log_info("At: %ld (%lx): ", address + 44, address);
			rtos::ThisThread::sleep_for(10ms);
			for (auto &elem: buffer) {
				printf("%x ", elem);
			}
			printf("\n");
		}
		rtos::ThisThread::sleep_for(1s);

		if (auto new_address = address + bytes_to_read; new_address > coreis25lp.getSize()) {
			address = 0x00;

			auto t = rtos::Kernel::Clock::now() - start;
			log_info("Firmware read in %is\n", int(t.count() / 1000));
			rtos::ThisThread::sleep_for(1s);
			start = rtos::Kernel::Clock::now();
		} else {
			address = new_address;
		}
	}
}
