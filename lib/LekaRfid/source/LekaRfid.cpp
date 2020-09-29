/**
 * @file LekaRfid.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 */

#include "LekaRfid.h"

RFID::RFID() : _interface(RFID_UART_TX, RFID_UART_RX, 57600) {}

bool RFID::checkConnected()
{
	uint8_t *buffer							   = new uint8_t[1];
	uint8_t buffer_length					   = 0;
	const uint8_t aimbed_buffer_length		   = 0x11;
	uint8_t aimed_buffer[aimbed_buffer_length] = {0x00, 0x0F, 0x4E, 0x46, 0x43, 0x20, 0x46, 0x53, 0x32,
												  0x4A, 0x41, 0x53, 0x54, 0x34, 0x00, 0x2A, 0xCE};

	_interface.write(_IDN_cmd, _IDN_cmd_length);
	ThisThread::sleep_for(1ms);

	for (int i = 0; i < 10; i++)
		if (_interface.readable()) {
			_interface.read(buffer, 2);
			if (buffer[0] == 0x00) {
				buffer_length = buffer[1];
				_interface.read(buffer + 2, buffer_length);
				if ((memcmp(aimed_buffer, buffer, aimbed_buffer_length) == 0)) {
					return true;
				}
			}
		}

	return false;
}

void RFID::start()
{
	printf("RFID example\n\n");

	while (!checkConnected()) {
		printf("RFID reader is not connected...\n");
		ThisThread::sleep_for(1s);
	}
	printf("RFID reader detected!\n\n");

	while (true) {
		ThisThread::sleep_for(10s);
	}

	printf("End of RFID example\n\n");
}
