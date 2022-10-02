// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaBluetooth.h"

using namespace mbed;
using namespace std::chrono;

Bluetooth::Bluetooth()
	: _interface(BT_UART_TX, BT_UART_RX, 115200), _bluetooth_reset(BT_RESET), _bluetooth_wake_up(BT_WAKE_UP)
{
	// rtos::ThisThread::sleep_for(1s);
	_bluetooth_wake_up = 1;
	rtos::ThisThread::sleep_for(25ms);	 // Delay recommanded in datasheet
	_bluetooth_reset = 1;
	rtos::ThisThread::sleep_for(450ms);	  // Delay recommanded in datasheet

	_buffer_length = BM64::getCommand(BM64::Command::eq_soft_mode, BM64::Command::eq_soft_mode_length, _buffer);
	_interface.write(_buffer, _buffer_length);
	rtos::ThisThread::sleep_for(100ms);
}

void Bluetooth::pairing()
{
	uint8_t max_attempts = 10;
	uint8_t attempts	 = 0;
	_buffer_length		 = 0;

	_buffer_length = BM64::getCommand(BM64::Command::pairing, BM64::Command::pairing_length, _buffer);

	while (attempts < max_attempts) {
		_interface.write(_buffer, _buffer_length);
		checkResponse();
		if (_paired) {
			break;
		} else {
			rtos::ThisThread::sleep_for(10s);
		}
		attempts++;
	}

	return;
}

void Bluetooth::checkResponse(bool printResponse)
{
	uint8_t attempts	 = 0;
	uint8_t max_attempts = 100;
	_buffer_length		 = 0;

	while (attempts < max_attempts) {
		rtos::ThisThread::sleep_for(10ms);
		while (_interface.readable()) {
			_interface.read(_buffer, 1);
			if (_buffer[0] == 0xAA) {
				_interface.read(_buffer + 1, 2);
				_buffer_length = ((uint16_t)_buffer[1] << 8) + (uint16_t)_buffer[2] + 1;   // +1 for checksum

				rtos::ThisThread::sleep_for(10ms);	 // Delay to catch length
				if (_buffer_length + 1 + 2 > max_buffer_size) {
					return;
				}

				_interface.read(_buffer + 1 + 2, _buffer_length);
				_buffer_length += 1 + 2;   // 0xAA + 2 bytes for length

				if (_buffer[3] == 0x01 && _buffer[4] == 0x06) {
					_paired = true;
				} else if (_buffer[3] == 0x22) {   // Value for message
					_msg_length = _buffer_length > max_buffer_size
									  ? max_buffer_size - 11
									  : _buffer_length - 11;   // 11 characters wrap the message
					for (uint16_t i = 0; i < _msg_length; i++) {
						_msg_rcv[i] = _buffer[i + 10];
					}
					_new_message = true;
				}

				if (printResponse) {
					printf("Frame received = ");
					for (int i = 0; i < _buffer_length; i++) {
						printf("%X ", _buffer[i]);
					}
					printf("\n");
				}
			}
			// attempts = 0;
		}
		attempts++;
	}

	return;
}

void Bluetooth::playPause()
{
	/* Pause or play video/song on paired device */

	_buffer_length = 0;

	_buffer_length = BM64::getCommand(BM64::Command::play_pause, BM64::Command::play_pause_length, _buffer);

	_interface.write(_buffer, _buffer_length);

	return;
}

bool Bluetooth::isPaired()
{
	return _paired;
}

void Bluetooth::sendMessage(char *msg, size_t msg_length)
{
	_buffer_length					= 0;
	const uint16_t msg_cmd_length	= 1 + 1 + 1 + 2 + 2 + msg_length;
	uint8_t msg_cmd[msg_cmd_length] = {
		0x12,
		0x00,
		0x00,
		(uint8_t)((uint16_t)msg_length >> 8),
		(uint8_t)msg_length,
		(uint8_t)((uint16_t)msg_length >> 8),
		(uint8_t)msg_length};	// Header of the command, check BM64 docs for more details

	// Copy message
	for (size_t i = 0; i < msg_length; i++) {
		msg_cmd[i + 7] = msg[i];
	}

	_buffer_length = BM64::getCommand(msg_cmd, msg_cmd_length, _buffer);

	_interface.write(_buffer, _buffer_length);
}

size_t Bluetooth::getMessage(char *buffer)
{
	for (uint16_t i = 0; i < _msg_length; i++) {
		buffer[i]	= _msg_rcv[i];
		_msg_rcv[i] = 0;
	}
	_new_message = false;
	return _msg_length;
}

bool Bluetooth::checkNewMessage()
{
	return _new_message;
}

void Bluetooth::start()
{
	char text[max_buffer_size] = {0};
	uint16_t text_length;

	printf("Bluetooth example\n\n");

	// Enable sound output
	_buffer_length = BM64::getCommand(BM64::Command::eq_soft_mode, BM64::Command::eq_soft_mode_length, _buffer);
	_interface.write(_buffer, _buffer_length);
	rtos::ThisThread::sleep_for(100ms);

	while (!isPaired()) {
		printf("Bluetooth is trying to pair...\n");
		pairing();
		rtos::ThisThread::sleep_for(1s);
	}
	text_length = sprintf(text, "This spike do an echo via bluetooth communication.\n");
	sendMessage(text, text_length);
	text_length = sprintf(text, "You also can connect with your phone and play songs via the robot!\nEnjoy!\n\n");
	sendMessage(text, text_length);

	while (true) {
		checkResponse(true);
		if (checkNewMessage()) {
			text_length = getMessage(text);
			rtos::ThisThread::sleep_for(2s);   // Delay of echo
			sendMessage(text, text_length);	   // ECHO
		}
		rtos::ThisThread::sleep_for(800ms);
	}

	printf("End of Bluetooth example\n\n");
}
