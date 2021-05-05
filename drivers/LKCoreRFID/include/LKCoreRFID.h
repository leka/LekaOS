// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

struct RFIDTag {
	uint8_t result_code {0};
	uint8_t length {0};
	uint8_t id[5] {0, 0, 0, 0, 0};
	uint8_t checks {0};
	uint8_t collisionbyte {0};
	uint8_t collisionbit {0};
};

class LKCoreRFID
{
  public:
	LKCoreRFID(interface::BufferedSerial &interface);
	auto RFIDMessageIntoStruct(uint8_t *tagValue, RFIDTag &rfid_tag) -> bool;
	auto writeProtocol() -> void;
	auto checkProtocol() -> bool;

  private:
	interface::BufferedSerial &_interface;

	auto isDataLengthOk(uint8_t length) -> bool;
	auto isResultCodeOk(uint8_t commabd) -> bool;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
