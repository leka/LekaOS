// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_RFID_H_
#define _LEKA_OS_DRIVER_LK_CORE_RFID_H_

#include <array>

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

// struct RFIDTag {
// 	uint8_t result_code {0};
// 	uint8_t length {0};
// 	uint8_t id[4] {0, 0, 0, 0};
// 	uint8_t check_sum {0};
// 	uint8_t checks {0};
// 	uint8_t collisionbyte {0};
// 	uint8_t collisionbit {0};
// };

struct RFIDTag {
	uint8_t UID[7] {0, 0, 0, 0, 0, 0, 0};
	uint8_t SAK[4] {0x00, 0x00, 0x00, 0x00};
	uint8_t data[16] {0};
};

class LKCoreRFID
{
  public:
	RFIDTag _rfid_tag {0, 0, 0};

	explicit LKCoreRFID(interface::BufferedSerial &interface);
	auto RFIDMessageIntoStruct(const uint8_t *tagValue, RFIDTag &rfid_tag) const -> void;
	auto writeProtocol() -> void;
	auto setGain() -> void;
	auto checkSensorSet() -> bool;

	auto sendREQA() -> void;
	auto checkATQA() -> bool;

	auto sendCL1() -> void;
	auto receiveUID1() -> void;

	auto sendUID1() -> void;
	// auto receiveSAK1() -> RFIDTag;

	// auto sendCL2() -> void;
	// auto receiveID2() -> RFIDTag;

	// auto sendID2() -> void;
	// auto receiveSAK2() -> RFIDTag;

	// auto authentification() -> bool;

	// auto readRFIDTag();

  private:
	interface::BufferedSerial &_interface;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
