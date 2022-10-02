// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BM64Converter.h"

inline constexpr uint8_t max_buffer_size = 128;

class Bluetooth
{
  public:
	Bluetooth();
	~Bluetooth() {};

	void start(void);
	void pairing();
	void playPause();
	void checkResponse(bool printResponse = false);

	bool isPaired();
	void sendMessage(char *msg, size_t msg_length);
	size_t getMessage(char *buffer);
	bool checkNewMessage();

  private:
	mbed::BufferedSerial _interface;
	mbed::DigitalOut _bluetooth_reset;
	mbed::DigitalOut _bluetooth_wake_up;

	uint8_t _buffer[max_buffer_size] = {0};
	uint16_t _buffer_length			 = 0;
	char _msg_rcv[max_buffer_size];
	size_t _msg_length = 0;

	bool _paired	  = false;	 // Check simultaneous pairing is possible
	bool _new_message = true;
};
