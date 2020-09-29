/**
 * @file LekaBluetooth.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-23
 *
 * @copyright Copyright (c) 2020
 */

#include "LekaBluetooth.h"

Bluetooth::Bluetooth()
	: _interface(BT_UART_TX, BT_UART_RX, 115200), _bluetooth_reset(BT_RESET), _bluetooth_wake_up(BT_WAKE_UP)
{
	// ThisThread::sleep_for(1s);
	_bluetooth_wake_up = 1;
	ThisThread::sleep_for(25ms);   // Delay recommanded in datasheet
	_bluetooth_reset = 1;
	ThisThread::sleep_for(450ms);	// Delay recommanded in datasheet
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
			ThisThread::sleep_for(10s);
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
		ThisThread::sleep_for(10ms);
		while (_interface.readable()) {
			_interface.read(_buffer, 1);
			if (_buffer[0] == 0xAA) {
				_interface.read(_buffer + 1, 2);
				_buffer_length = ((uint16_t)_buffer[1] << 8) + (uint16_t)_buffer[2] + 1;

				_interface.read(_buffer + 1 + 2, _buffer_length);
				_buffer_length += 1 + 2;

				if (_buffer[3] == 0x01 && _buffer[4] == 0x06) {
					_paired = true;
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

// void Bluetooth::converse() {}

void Bluetooth::start()
{
	printf("Bluetooth example\n\n");

	while (true) {
		pairing();
		while (_paired) {
			checkResponse(true);
			ThisThread::sleep_for(20ms);
			// playPause();
			// ThisThread::sleep_for(5s);
		}
		// converse();
		ThisThread::sleep_for(10s);
	}

	printf("End of Bluetooth example\n\n");
}
