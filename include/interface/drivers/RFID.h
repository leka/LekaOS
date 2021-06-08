// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
#define _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_

#include "mbed.h"
#include <cstdint>
#include <functional>

using tagAvailableCallback = Callback<void()>;

#include "BufferedSerial.h"

namespace leka {

namespace rfid {

	struct Tag {
		std::array<uint8_t, 7> id {};
		std::array<uint8_t, 4> sak {};
		std::array<uint8_t, 16> data {};
	};

	enum class Flag : uint8_t
	{
		sb_7 = 0b00000111,	 // significant bits : 7
		sb_8 = 0b00001000,	 // significant bits : 8
		crc	 = 0b00100000
	};

	constexpr Flag operator|(Flag lhs, Flag rhs)
	{
		return static_cast<Flag>(static_cast<std::underlying_type_t<Flag>>(lhs) |
								 static_cast<std::underlying_type_t<Flag>>(rhs));
	};

	enum class Protocol : uint8_t
	{
		ISO14443A,
		ISO14443B,
		ISO15693,
		ISO18092
	};

}	// namespace rfid

namespace interface {

	class RFID
	{
	  public:
		virtual ~RFID() = default;

		virtual void init() = 0;

		virtual void registerTagAvailableCallback(tagAvailableCallback callback) = 0;
		virtual void onDataAvailable()											 = 0;

		virtual auto getIDN(std::array<uint8_t, 17> &) -> bool = 0;
		virtual auto setBaudrate(uint8_t baudrate) -> bool	   = 0;

		virtual auto setCommunicationProtocol(rfid::Protocol protocol) -> bool = 0;

		virtual void sendCommandToTag(lstd::span<uint8_t> cmd) = 0;

		virtual auto receiveDataFromTag(lstd::span<uint8_t> data) -> bool = 0;

		virtual void setModeTagDetection() = 0;

		virtual auto checkForTagDetection() -> bool = 0;

		std::array<uint8_t, 32> _rx_buf {};

		class ISO14443
		{
		  public:
			virtual void init() = 0;

			virtual void getTagData() = 0;

			virtual auto getTag() -> rfid::Tag = 0;

			template <size_t SIZE>
			struct Command {
				const std::array<uint8_t, SIZE> data;
				const leka::rfid::Flag flags;
				[[nodiscard]] inline auto getArray() const -> std::array<uint8_t, SIZE + 1>
				{
					auto cmd = std::array<uint8_t, SIZE + 1> {};

					for (int i = 0; i < SIZE; ++i) {
						cmd[i] = data[i];
					}

					cmd[SIZE] = flags;

					return cmd;
				}
			};

		  protected:
			Command<1> command_requestA		   = {.data = {0x26}, .flags = leka::rfid::Flag::sb_7};
			Command<2> command_read_register_0 = {.data	 = {0x30, 0x00},
												  .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};
			Command<2> command_read_register_4 = {.data	 = {0x30, 0x04},
												  .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};
			Command<2> command_read_register_6 = {.data	 = {0x30, 0x06},
												  .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};

			std::array<uint8_t, 2> ATQA_answer {0x44, 0x00};

			std::array<uint8_t, 18> _tag_data {0};
		};
	};

}	// namespace interface
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
