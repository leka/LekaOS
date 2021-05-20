// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_RFID_H_
#define _LEKA_OS_DRIVER_CORE_RFID_H_

#include <array>

#include "drivers/BufferedSerial.h"

#include "CoreBufferedSerial.h"

namespace leka {

class CoreRFID
{
  public:
	struct Tag {
		std::array<uint8_t, 8> uid {0};
		std::array<uint8_t, 2> crc_uid {0};
		std::array<uint8_t, 4> sak {0};
		std::array<uint8_t, 16> data {0};
	};

  public:
	explicit CoreRFID(interface::BufferedSerial &interface);

	auto setRFIDTag(std::array<uint8_t, 8> &tag_uid, std::array<uint8_t, 2> &tag_crc_uid,
					std::array<uint8_t, 4> &tag_sak, std::array<uint8_t, 16> &tag_data) -> void;

	auto getTag() const -> CoreRFID::Tag;

	template <size_t N>
	void send(const std::array<uint8_t, N> &command);

	auto setProtocol() -> void;
	auto setGain() -> void;
	auto receiveSetupAnswer() -> bool;

	auto sendREQA() -> void;
	auto receiveATQA() -> bool;

	auto sendCL1() -> void;
	auto sendCL2() -> void;

	auto receiveUID1() -> void;
	auto receiveUID2() -> void;

	auto sendUID1() -> void;
	auto sendUID2() -> void;

	auto receiveSAK1() -> void;
	auto receiveSAK2() -> void;

	auto authentification() -> void;
	auto receiveAuthentification() -> bool;

	auto readRFIDTag() -> void;
	auto receiveRFIDTag() -> void;

  private:
	interface::BufferedSerial &_interface;
	CoreRFID::Tag my_tag;

	auto checkSensorSetup(const uint8_t *buffer) const -> bool;
	auto checkATQA(const uint8_t *buffer) const -> bool;

	auto setUID1(uint8_t *buffer) -> void;
	auto setUID2(uint8_t *buffer) -> void;

	auto setSAK1(uint8_t *buffer) -> void;
	auto setSAK2(uint8_t *buffer) -> void;

	auto checkAuthentification(const uint8_t *buffer) const -> bool;

	auto computeCrcIso14443a(uint8_t const *pbtData, size_t szLen) const -> std::array<uint8_t, 2>;
	auto checkCRC(const std::array<uint8_t, 21> &buffer) const -> bool;

	auto setData(uint8_t *buffer) -> void;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_RFID_H_
