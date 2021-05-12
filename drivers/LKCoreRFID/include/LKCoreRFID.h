// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include <array>

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

struct RFIDTag {
	uint8_t UID[8] {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t crc_UID[2] {0, 0};
	uint8_t SAK[4] {0, 0, 0, 0};
	uint8_t data[16] {0};
};

class LKCoreRFID
{
  public:
	RFIDTag _rfid_tag {0, 0, 0, 0};

	explicit LKCoreRFID(interface::BufferedSerial &interface);

	auto setRFIDTag(RFIDTag expected_values) -> void;

	auto writeProtocol() -> void;
	auto setGain() -> void;
	auto checkSensorSet() -> bool;

	auto sendREQA() -> void;
	auto checkATQA() -> bool;

	auto sendCL1() -> void;
	auto receiveUID1() -> void;

	auto sendUID1() -> void;
	auto receiveSAK1() -> void;

	auto sendCL2() -> void;
	auto receiveUID2() -> void;

	auto sendUID2() -> void;
	auto receiveSAK2() -> void;

	// auto authentification() -> bool;

	// auto readRFIDTag();

	// auto checkCRC() -> bool;

  private:
	interface::BufferedSerial &_interface;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
