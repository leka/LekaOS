// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include "drivers/BufferedSerial.h"

#define _MAXIMUM_BUFFER_SIZE 253   // 255 minus 1 bit for command and one for length

namespace leka {

struct RFIDTag {
	uint8_t command;
	uint8_t length;
	uint8_t data[_MAXIMUM_BUFFER_SIZE];
};

class LKCoreRFID
{
  public:
	LKCoreRFID(mbed::BufferedSerial &interface);
	auto RFIDMessageIntoStruct(uint8_t *tagValue) -> RFIDTag;
	// auto setProtocol();

  private:
	mbed::BufferedSerial &_interface;
	auto isDataLengthOk(uint8_t length) -> bool;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
