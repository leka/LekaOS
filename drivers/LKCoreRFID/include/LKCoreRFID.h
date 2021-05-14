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

	template <size_t N>
	void send(const std::array<uint8_t, N> &command);

	auto setRFIDTag(RFIDTag *expected_values) -> void;
	auto getRFIDTag() const -> RFIDTag;

	auto setProtocol() -> void;
	auto setGain() -> void;
	auto receiveSetupAnswer() -> bool;

	auto sendREQA() -> void;
	auto receiveATQA() -> bool;

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

	auto checkATQA(const uint8_t *buffer) const -> bool;
	auto checkSensorSetup(const uint8_t *buffer) const -> bool;

	auto setUID1(uint8_t *buffer) -> void;
	auto setUID2(uint8_t *buffer) -> void;

	auto setSAK1(uint8_t *buffer) -> void;
	auto setSAK2(uint8_t *buffer) -> void;

	auto setData(uint8_t *buffer) -> void;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_RFID_H_
