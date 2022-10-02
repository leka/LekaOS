// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "rtos/ThisThread.h"

#include "CoreFlashUtils.h"

using namespace leka;
using namespace std::chrono;

CoreQSPI coreqspi {};
CoreFlashManagerIS25LP016D coremanageris25lp(coreqspi);
CoreFlashIS25LP016D coreis25lp(coreqspi, coremanageris25lp);

uint32_t address = 0x00;

const size_t bytes_to_read = 0x20;
std::array<uint8_t, bytes_to_read> buffer {};

auto data_to_write = std::to_array<uint8_t>({1, 2, 3, 4, 5, 6, 7, 8, 9});

void flash_loop()
{
	coreis25lp.reset();

	rtos::ThisThread::sleep_for(2s);

	coreqspi.setDataTransmissionFormat();
	coreqspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);

	coreis25lp.erase();

	while (true) {
		rtos::ThisThread::sleep_for(200ms);

		auto bytes_written = coreis25lp.write(address, data_to_write, std::size(data_to_write));
		coreis25lp.read(address, buffer, bytes_written);

		if (auto new_address = address + bytes_to_read; new_address > coreis25lp.getSize()) {
			address = 0x00;
		} else {
			address = new_address;
		}
	}
}
