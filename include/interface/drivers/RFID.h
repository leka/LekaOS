// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
#define _LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_

namespace leka {

namespace rfid {

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

}	// namespace rfid

namespace interface {

	class RFID
	{
	  public:
		virtual ~RFID() = default;

		virtual auto init() -> bool = 0;

		virtual void send(const lstd::span<uint8_t> &iso_command) = 0;

		virtual auto receive(const lstd::span<uint8_t> &tag_anwser) -> size_t = 0;

		class ISO14443
		{
		  public:
			void virtual write() = 0;
			void virtual read()	 = 0;

			template <size_t SIZE>
			struct Command {
				const std::array<uint8_t, SIZE> data;
				const leka::rfid::Flag flags;
				[[nodiscard]] inline auto getData() const -> uint8_t * { return data.data(); }
			};

		  private:
			std::array<uint8_t, 16> _tag_data {0};

			Command<1> command_requestA		   = {.data = {0x26}, .flags = leka::rfid::Flag::sb_7};
			Command<2> command_read_register_8 = {.data	 = {0x30, 0x08},
												  .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};
		};
	};

}	// namespace interface
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_KIT_RFID_INTERFACE_H_
