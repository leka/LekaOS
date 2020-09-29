/**
 * @file LekaBluetooth.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-23
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_BLUETOOTH_H_
#define _LEKA_OS_LIB_BLUETOOTH_H_

#include "BM64Converter.h"
#include "LekaPinNames.h"
#include "mbed.h"

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
	BufferedSerial _interface;
	DigitalOut _bluetooth_reset;
	DigitalOut _bluetooth_wake_up;

	uint8_t _buffer[24]		= {0};
	uint16_t _buffer_length = 0;

	bool _paired = false;	// Check simultaneous pairing is possible
};

#endif
