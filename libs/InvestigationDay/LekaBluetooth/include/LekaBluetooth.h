// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLUETOOTH_H_
#define _LEKA_OS_LIB_BLUETOOTH_H_

#include <chrono>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BM64Converter.h"

class Bluetooth
{
  public:
	Bluetooth();
	~Bluetooth() {};

	void start(void);
	void pairing();
	void playPause();
	void checkResponse(bool printResponse = false);
	// void converse();

  private:
	mbed::BufferedSerial _interface;
	mbed::DigitalOut _bluetooth_reset;
	mbed::DigitalOut _bluetooth_wake_up;

	uint8_t _buffer[24]		= {0};
	uint16_t _buffer_length = 0;

	bool _paired = false;	// Check simultaneous pairing is possible
};

#endif
