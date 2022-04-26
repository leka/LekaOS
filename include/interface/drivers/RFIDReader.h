// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>
#include <span>

#include "platform/Callback.h"

using tag_available_callback_t = mbed::Callback<void()>;

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
		crc	 = 0b00100000,
	};

	constexpr auto operator|(Flag lhs, Flag rhs) -> Flag
	{
		return static_cast<Flag>(static_cast<std::underlying_type_t<Flag>>(lhs) |
								 static_cast<std::underlying_type_t<Flag>>(rhs));
	};

	enum class Protocol : uint8_t
	{
		ISO14443A,
		ISO14443B,
		ISO15693,
		ISO18092,
	};

}	// namespace rfid

namespace interface {

	class RFIDReader
	{
	  public:
		virtual ~RFIDReader() = default;

		virtual void init() = 0;

		virtual void registerTagAvailableCallback(tag_available_callback_t callback) = 0;
		virtual void onDataAvailable()												 = 0;

		virtual auto setBaudrate(uint8_t baudrate) -> bool = 0;

		virtual auto setCommunicationProtocol(rfid::Protocol protocol) -> bool = 0;

		virtual void sendCommandToTag(std::span<const uint8_t> cmd) = 0;

		virtual auto receiveDataFromTag(std::span<uint8_t> data) -> bool = 0;

		virtual void setTagDetectionMode() = 0;

		virtual auto isTagDetected() -> bool = 0;

		class ISO14443
		{
		  public:
			virtual ~ISO14443() = default;

			virtual void init() = 0;

			virtual void runStateMachine() = 0;

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

					cmd[SIZE] = static_cast<uint8_t>(flags);

					return cmd;
				}
			};

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
