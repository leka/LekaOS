// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaRFID.h"

using namespace mbed;
using namespace std::chrono;

RFID::RFID() : _interface(RFID_UART_TX, RFID_UART_RX, 57600) {}

bool RFID::echo()
{
	uint8_t buffer[1]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x1;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x55};

	_interface.write(_echo_cmd, _echo_cmd_length);
	rtos::ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
				return true;
			}
		}
	}
	return false;
}

bool RFID::checkConnected()
{
	return echo();
}

bool RFID::setSync()
{
	uint8_t buffer[2]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x2;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x00};

	_interface.write(_set_receiver_gain_cmd, _set_receiver_gain_cmd_length);
	rtos::ThisThread::sleep_for(10ms);

	// TODO: why a for loop? why not while(!_interface.readable())?
	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			// TODO: check the second for loop and the use of the index
			for (int j = 0; j < aimed_buffer_length; j++) {
				printf("%X ", buffer[j]);
			}
			printf("\n");
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
				return true;
			}
		}
	}
	return false;
}

bool RFID::setReceiverGain()
{
	uint8_t buffer[2]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x2;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x00};

	_interface.write(_set_receiver_gain_cmd, _set_receiver_gain_cmd_length);
	rtos::ThisThread::sleep_for(10ms);

	// TODO: why a for loop? why not while(!_interface.readable())?
	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			// TODO: check the second for loop and the use of the index
			for (int j = 0; j < aimed_buffer_length; j++) {
				printf("%X ", buffer[j]);
			}
			printf("\n");
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
				return true;
			}
		}
	}
	return false;
}

void RFID::sendReceive(uint8_t val)
{
	const uint8_t max_buffer_length	  = 0x20;	// TODO: what is the maximum length that we can receive?
	uint8_t buffer[max_buffer_length] = {0};

	if (val == 1) {
		_interface.write(_send_receive_cmd, _send_receive_cmd_length);
	} else if (val == 2) {
		_interface.write(_send_receive2_cmd, _send_receive2_cmd_length);
	} else if (val == 3) {
		_interface.write(_send_receive3_cmd, _send_receive3_cmd_length);
	} else if (val == 4) {
		_interface.write(_send_receive4_cmd, _send_receive4_cmd_length);
	} else if (val == 5) {
		_interface.write(_send_receive5_cmd, _send_receive5_cmd_length);
	} else if (val == 6) {
		_interface.write(_send_receive6_cmd, _send_receive6_cmd_length);
	} else if (val == 7) {
		_interface.write(_send_receive7_cmd, _send_receive7_cmd_length);
	} else if (val == 8) {
		_interface.write(_send_receive8_cmd, _send_receive8_cmd_length);
	} else if (val == 9) {
		_interface.write(_send_receive9_cmd, _send_receive9_cmd_length);
	}
	rtos::ThisThread::sleep_for(10ms);

	// TODO: why a for loop? why not while(!_interface.readable())?
	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, 2);
			auto length = buffer[1];
			_interface.read(&buffer[2], length);

			// TODO: check the second for loop and the use of the index
			_answer_length = length + 2;
			for (int j = 0; j < length + 2; j++) {
				_answer[j] = buffer[j];
				printf("%X ", buffer[j]);
			}
			printf("\n");
			return;
		}
	}
	printf("No answer received from reader...\n");
	return;
}
size_t RFID::getAnswer(char *buffer)
{
	for (uint16_t i = 0; i < _answer_length; i++) {
		buffer[i] = _answer[i];
	}
	return _answer_length;
}

bool RFID::setIEC14443()
{
	uint8_t buffer[2]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x02;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x00};

	_interface.write(_iec_14443_cmd, _iec_14443_cmd_length);
	rtos::ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
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
		rtos::ThisThread::sleep_for(1s);
	}
	printf("RFID reader detected!\n\n");

	while (!setIEC14443()) {
		printf("Attempt to enable RFID reader...\n");
		rtos::ThisThread::sleep_for(1s);
	}
	printf("RFID reader enable with IEC 14443!\n");

	while (!setSync()) {
		printf("Attempt to set RFID sync...\n");
		rtos::ThisThread::sleep_for(1s);
	}

	while (!setReceiverGain()) {
		printf("Attempt to set RFID reader gain...\n");
		rtos::ThisThread::sleep_for(1s);
	}
	printf("RFID reader gain set!\n");

	while (true) {
		sendReceive(2);
		sendReceive(3);
		sendReceive(4);
		sendReceive(5);
		sendReceive(6);
		sendReceive(7);
		sendReceive(8);
		sendReceive(9);
		rtos::ThisThread::sleep_for(1s);
	}

	printf("End of RFID example\n\n");
}
