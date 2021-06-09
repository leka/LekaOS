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

	void sendREQA() final;
	void sendReadRegister8() final;

	void read() final;

  private:
	interface::RFID &_rfid_reader;

	template <size_t SIZE>
	void commandToArray(Command<SIZE> command, const lstd::span<uint8_t> &span)
	{
		for (auto i = 0; i < SIZE; ++i) {
			span.data()[i] = command.data.data()[i];
		}

		span.data()[SIZE] = static_cast<uint8_t>(command.flags);
	}
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_RFID_KIT_H_
