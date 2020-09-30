/**
 * @file LekaBluetooth.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_RFID_H_
#define _LEKA_OS_LIB_RFID_H_

#include "LekaPinNames.h"
#include "mbed.h"

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
	bool setReceiverGain();
	void sendReceive();

  private:
	BufferedSerial _interface;

	const uint8_t _echo_cmd[1]					= {0x55};
	const uint8_t _echo_cmd_length				= 1;
	const uint8_t _idn_cmd[2]					= {0x01, 0x00};
	const uint8_t _idn_cmd_length				= 2;
	const uint8_t _field_off_cmd[4]				= {0x02, 0x02, 0x00, 0x00};
	const uint8_t _field_off_cmd_length			= 4;
	const uint8_t _iec_15693_cmd[4]				= {0x02, 0x02, 0x01, 0x09};
	const uint8_t _iec_15693_cmd_length			= 4;
	const uint8_t _set_receiver_gain_cmd[6]		= {0x09, 0x04, 0x68, 0x01, 0x01, 0xD1};
	const uint8_t _set_receiver_gain_cmd_length = 6;
	const uint8_t _send_receive_cmd[5]			= {0x04, 0x03, 0x26, 0x01, 0x00};
	const uint8_t _send_receive_cmd_length		= 5;
};

#endif
