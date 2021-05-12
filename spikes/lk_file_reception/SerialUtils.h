// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

using namespace std::chrono;

auto receptionBySerial(mbed::BufferedSerial &serial, FIL &file) -> uint32_t
{
	uint8_t buffer[0x100] = {0};
	uint8_t buffer_size	  = 200;

	uint32_t application_size = 0x0;
	int wd					  = 0;	 // Similar to "Watchdog"
	UINT nbBytesWritten {0};

	while (!(serial.readable())) {
		rtos::ThisThread::sleep_for(1s);
	}

	if (serial.readable()) {
		serial.read(buffer, 0x04);
		application_size = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
	}

	serial.write("K", 1);	// ACK to sender
	while (wd < 0x1000) {
		if (serial.readable()) {
			auto remaining = application_size - f_size(&file);
			buffer_size	   = (remaining < buffer_size) ? remaining : buffer_size;

			serial.read(buffer, buffer_size);

			f_write(&file, buffer, buffer_size, &nbBytesWritten);

			if (f_size(&file) >= application_size) {
				serial.write("E", 1);	// ACK to sender
				break;
			}

			wd = 0;
			serial.write("K", 1);	// ACK to sender
		} else {
			wd++;
		}
	}

	return application_size;
}
