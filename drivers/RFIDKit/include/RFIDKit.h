// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_RFID_KIT_H_
#define _LEKA_OS_DRIVER_LK_RFID_KIT_H_

#include <cstdint>
#include <lstd_span>

#include "interface/drivers/RFID.h"

namespace leka {

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
	void sendReadRegister8();

	auto receiveATQA() -> bool;
	auto receiveReadTagData() -> bool;
	auto computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>;

	void getData(std::array<uint8_t, 16> &tag_data);

	interface::RFID &_rfid_reader;
	rfid::Tag _tag {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_RFID_KIT_H_
