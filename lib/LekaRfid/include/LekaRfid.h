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

  private:
};

#endif
