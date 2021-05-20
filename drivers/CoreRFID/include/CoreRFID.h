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

	explicit CoreRFID(interface::BufferedSerial &interface);

	void setTag(CoreRFID::Tag const &tag);
	auto getTag() const -> CoreRFID::Tag;

	template <size_t N>
	void send(const std::array<uint8_t, N> &command);

	void setProtocol();
	void setGain();
	auto receiveSetupAnswer() -> bool;

	void sendREQA();
	auto receiveATQA() -> bool;

	void sendCL1();
	void sendCL2();

	void receiveUID1();
	void receiveUID2();

	void sendUID1();
	void sendUID2();

	void receiveSAK1();
	void receiveSAK2();

	void authentication();
	auto receiveAuthentication() -> bool;

	void readTagData();
	void receiveTagData();

  private:
	interface::BufferedSerial &_interface;
	CoreRFID::Tag my_tag;

	auto checkSensorSetup(const uint8_t *buffer) const -> bool;
	auto checkATQA(const uint8_t *buffer) const -> bool;

	void setUID1(uint8_t *buffer);
	void setUID2(uint8_t *buffer);

	void setSAK1(uint8_t *buffer);
	void setSAK2(uint8_t *buffer);

	auto checkAuthentication(const uint8_t *buffer) const -> bool;

	auto computeCRC(uint8_t const *data, size_t size) const -> std::array<uint8_t, 2>;
	auto checkCRC(const std::array<uint8_t, 21> &buffer) const -> bool;

	void setData(uint8_t *buffer);
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_RFID_H_
