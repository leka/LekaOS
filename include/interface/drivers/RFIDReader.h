// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>
#include <span>

namespace leka {

namespace rfid {

	struct Tag {
		std::array<uint8_t, 7> id {};
		std::array<uint8_t, 4> sak {};
		std::array<uint8_t, 18> data {};
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

		virtual void init()																  = 0;
		virtual void setTagDetectionMode()												  = 0;
		virtual auto isTagDetected() -> bool											  = 0;
		virtual void onTagDataReceived()												  = 0;
		virtual auto getTag() -> rfid::Tag												  = 0;
		virtual auto getDataFromTag() -> std::array<uint8_t, 18>						  = 0;
		virtual auto didTagCommunicationSucceed(size_t sizeTagData) -> bool				  = 0;
		virtual void sendToTag(std::span<const uint8_t> data)							  = 0;
		virtual void setCommunicationProtocol(rfid::Protocol protocol)					  = 0;
		virtual void registerOnTagDetectedCallback(const std::function<void()> &callback) = 0;
		virtual void registerOnTagReadableCallback(const std::function<void()> &callback) = 0;
	};

}	// namespace interface
}	// namespace leka
