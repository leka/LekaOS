// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

class RFID
{
  public:
	RFID();
	~RFID() {};

	void start(void);

	bool checkConnected();

	bool echo();
	bool getID();
	void fieldOff();
	bool setIEC15693();
	bool setIEC14443();
	bool setReceiverGain();
	void sendReceive(uint8_t val);

	size_t getAnswer(char *buffer);

  private:
	mbed::BufferedSerial _interface;

	const uint8_t _echo_cmd[1]					= {0x55};
	const uint8_t _echo_cmd_length				= 1;
	const uint8_t _idn_cmd[2]					= {0x01, 0x00};
	const uint8_t _idn_cmd_length				= 2;
	const uint8_t _field_off_cmd[4]				= {0x02, 0x02, 0x00, 0x00};
	const uint8_t _field_off_cmd_length			= 4;
	const uint8_t _iec_15693_cmd[4]				= {0x02, 0x02, 0x01, 0x09};
	const uint8_t _iec_15693_cmd_length			= 4;
	const uint8_t _iec_14443_cmd[4]				= {0x02, 0x02, 0x02, 0x00};
	const uint8_t _iec_14443_cmd_length			= 4;
	const uint8_t _set_receiver_gain_cmd[6]		= {0x09, 0x04, 0x68, 0x01, 0x01, 0xD1};
	const uint8_t _set_receiver_gain_cmd_length = 6;
	const uint8_t _send_receive_cmd[5]			= {0x04, 0x03, 0x26, 0x01, 0x00};
	const uint8_t _send_receive_cmd_length		= 5;
	const uint8_t _send_receive2_cmd[4]			= {0x04, 0x02, 0x26, 0x07};
	const uint8_t _send_receive2_cmd_length		= 4;
	const uint8_t _send_receive3_cmd[5]			= {0x04, 0x03, 0x93, 0x20, 0x08};
	const uint8_t _send_receive3_cmd_length		= 5;

	uint8_t _answer[64];
	size_t _answer_length = 0;
};
