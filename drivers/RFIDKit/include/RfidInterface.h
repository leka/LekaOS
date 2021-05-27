// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
#define _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace leka::interface {

enum class Flag : uint8_t
{
	b7	= 0b00000111,
	b8	= 0b00001000,
	crc = 0b00100000,
};

constexpr Flag operator|(Flag lhs, Flag rhs)
{
	return static_cast<Flag>(static_cast<std::underlying_type_t<Flag>>(lhs) |
							 static_cast<std::underlying_type_t<Flag>>(rhs));
}
template <size_t SIZE>
struct CommandISO {
	[[nodiscard]] auto getData() const -> const uint8_t * { return data.data(); }
	std::array<uint8_t, SIZE> data;
	Flag flags;
};

class RFID
{
  public:
	virtual ~RFID() = default;

	template <size_t SIZE>
	void send(CommandISO<SIZE> cmd);
	virtual void receive(uint8_t *data, size_t size) = 0;

	class ISO14443
	{
	  public:
		virtual void requestA();
		virtual void read();

	  private:
		std::array<uint8_t, 16> _tag_data {0};

		CommandISO<1> command_requestA		  = {.data = {0x26}, .flags = Flag::b7};
		CommandISO<2> command_read_register_8 = {.data = {0x30, 0x08}, .flags = Flag::crc | Flag::b8};
	};
};

}	// namespace leka::interface

#endif	 //_LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
