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
	std::array<uint8_t, 8> UID {0};
	std::array<uint8_t, 2> crc_UID {0};
	std::array<uint8_t, 4> SAK {0};
	std::array<uint8_t, 16> data {0};
};

class LKCoreRFID
{
  public:
	explicit LKCoreRFID(interface::BufferedSerial &interface);

	auto setRFIDTag(RFIDTag &expected_values) -> void;
	auto getRFIDTag() -> RFIDTag;

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

	auto authentification() -> void;

	auto readRFIDTag() -> void;
	auto receiveRFIDTag() -> void;
	// auto checkCRC() -> bool;

  private:
	interface::BufferedSerial &_interface;
	RFIDTag _rfid_tag {0, 0, 0, 0};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
