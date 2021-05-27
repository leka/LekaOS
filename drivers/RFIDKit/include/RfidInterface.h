// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
#define _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_

#include <array>
#include <cstddef>
#include <stdint.h>

namespace ISO14443_command {
constexpr uint8_t request_A = 0x26;
constexpr std::array<uint8_t, 2> read_register_8 {0x30, 0x08};
}	// namespace ISO14443_command

namespace leka::interface {

class RFID
{
  public:
	virtual ~RFID() = default;

	virtual void send(uint8_t *data, const size_t size) = 0;
	virtual void receive(uint8_t *data, size_t size)	= 0;

	class ISO14443
	{
	  public:
		virtual void requestA();
		virtual void read();

	  private:
		std::array<uint8_t, 16> _tag_data {0};
	};
};

}	// namespace leka::interface

#endif	 //_LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
