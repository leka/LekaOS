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

  private:
	BufferedSerial _interface;

	const uint8_t _IDN_cmd[2]	  = {0x00, 0x01};
	const uint8_t _IDN_cmd_length = 2;
};

#endif
