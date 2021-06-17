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
	auto getTagData(std::array<uint8_t, 16> &tag_data) -> bool final;

  private:
	template <size_t SIZE>
	void commandToArray(Command<SIZE> command, const lstd::span<uint8_t> &span) const
	{
		for (auto i = 0; i < SIZE; ++i) {
			span.data()[i] = command.data.data()[i];
		}

		span.data()[SIZE] = static_cast<uint8_t>(command.flags);
	}

	void sendREQA();
	void sendReadRegister8();

	auto receiveATQA() -> bool;
	auto receiveTagData() -> bool;
	auto computeCRC(uint8_t const *data, size_t size) const -> std::array<uint8_t, 2>;

	void getData(std::array<uint8_t, 16> &tag_data);

	interface::RFID &_rfid_reader;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_RFID_KIT_H_
