// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_RFID_KIT_H_
#define _LEKA_OS_DRIVER_LK_RFID_KIT_H_

#include <cstdint>
#include <lstd_span>

#include "interface/drivers/RFID.h"

namespace leka {

enum class state : uint8_t
{

	SENSOR_WAKE_UP				   = 0x00,
	TAG_COMMUNICATION_PROTOCOL_SET = 0x01,
	WAIT_FOR_ATQA_RESPONSE		   = 0x02,
	TAG_IDENTIFIED				   = 0x03,
	TAG_ACTIVATED				   = 0x04,

};

class RFIDKit : public interface::RFID::ISO14443
{
  public:
	explicit RFIDKit(interface::RFID &rfid_reader) : _rfid_reader(rfid_reader) {};

	void init() final;

	void getTagData() final;

	auto getTag() -> rfid::Tag final { return _tag; };

  private:
	template <size_t SIZE>
	void commandToArray(Command<SIZE> command, lstd::span<uint8_t> span) const
	{
		for (size_t i = 0; i < SIZE; ++i) {
			span.data()[i] = command.data.data()[i];
		}

		span.data()[SIZE] = static_cast<uint8_t>(command.flags);
	}

	void sendREQA();
	void sendReadRegister0();
	void sendReadRegister6();
	void sendWriteRegister(uint8_t registerToWrite, std::array<uint8_t, 4> data);

	auto receiveATQA() -> bool;
	auto receiveReadTagData() -> bool;
	void receiveWriteTagData();

	void sendAuthentificate();
	void receiveAuthentificate();

	auto computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>;

	void getData(std::array<uint8_t, 16> &tag_data);

	interface::RFID &_rfid_reader;
	rfid::Tag _tag {};
	state _state = state::SENSOR_WAKE_UP;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_RFID_KIT_H_
