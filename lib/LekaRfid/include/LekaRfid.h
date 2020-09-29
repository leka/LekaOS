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
	void fieldOff();
	bool setIEC15693();

  private:
	BufferedSerial _interface;

	const uint8_t _echo_cmd[2]			= {0x55};
	const uint8_t _echo_cmd_length		= 1;
	const uint8_t _idn_cmd[2]			= {0x01, 0x00};
	const uint8_t _idn_cmd_length		= 2;
	const uint8_t _field_off_cmd[4]		= {0x02, 0x02, 0x00, 0x00};
	const uint8_t _field_off_cmd_length = 4;
	const uint8_t _iec_15693_cmd[4]		= {0x02, 0x02, 0x01, 0x09};
	const uint8_t _iec_15693_cmd_length = 4;
};

#endif
