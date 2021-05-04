// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

struct RFIDTag {
	uint8_t result_code;
	uint8_t length;
	uint8_t id[5];
	uint8_t checks;
	uint8_t collisionbyte;
	uint8_t collisionbit;
};

class LKCoreRFID
{
  public:
	LKCoreRFID(interface::BufferedSerial &interface);

	auto writeProtocol() -> void;
	auto checkProtocol() -> bool;

  private:
	interface::BufferedSerial &_interface;
	auto RFIDMessageIntoStruct(uint8_t *tagValue) -> RFIDTag;
	auto isDataLengthOk(uint8_t length) -> bool;
	auto isResultCodeOk(uint8_t commabd) -> bool;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
