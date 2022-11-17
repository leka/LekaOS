// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>

#include "boost/sml.hpp"
#include "interface/drivers/RFIDReader.h"

namespace leka::rfid {

template <std::size_t SIZE>
struct Command {
	struct Details {
		std::initializer_list<uint8_t> data;
		rfid::Flag flags {};
	};

	explicit constexpr Command(Details details)
	{
		std::copy(details.data.begin(), details.data.end(), _data.begin());
		_data.back() = static_cast<uint8_t>(details.flags);
	};

	[[nodiscard]] constexpr auto data() const -> std::span<const uint8_t> { return std::span(_data); }

  private:
	std::array<uint8_t, SIZE + 1> _data;
};

inline constexpr auto command_requestA = Command<1>({.data = {0x26}, .flags = leka::rfid::Flag::sb_7});
inline constexpr auto command_read_register_4 =
	Command<2>({.data = {0x30, 0x04}, .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8});

inline constexpr auto ATQA_answer_size		   = 2;
inline constexpr auto initial_polynomial_value = uint32_t {0x6363};
inline constexpr auto register_answer_size	   = std::size_t {18};
inline constexpr auto expected_ATQA_answer	   = std::array<uint8_t, ATQA_answer_size> {0x44, 0x00};

inline auto computeCRC(uint8_t const *data) -> std::array<uint8_t, 2>
{
	// ? Implementation taken from libnfc:
	// ? https://github.com/nfc-tools/libnfc/blob/master/libnfc/iso14443-subr.c#L47-L62

	uint32_t wCrc = initial_polynomial_value;
	size_t size	  = 16;

	do {
		std::byte val {};
		val	 = static_cast<std::byte>(*data++);
		val	 = (val ^ static_cast<std::byte>(wCrc & 0x00FF));
		val	 = (val ^ (val << 4));
		wCrc = (wCrc >> 8) ^ (static_cast<uint32_t>(val) << 8) ^ (static_cast<uint32_t>(val) << 3) ^
			   (static_cast<uint32_t>(val) >> 4);
	} while (--size != 0);

	std::array<uint8_t, 2> pbtCrc = {static_cast<uint8_t>(wCrc & 0xFF), static_cast<uint8_t>((wCrc >> 8) & 0xFF)};
	return pbtCrc;
}

inline auto checkATQA(rfid::Tag tag) -> bool
{
	auto is_atqa_correct = std::equal(expected_ATQA_answer.begin(), expected_ATQA_answer.end(), tag.data.begin());
	return is_atqa_correct;
}

inline auto checkRegister(rfid::Tag tag) -> bool
{
	auto crc_computed	= computeCRC(tag.data.data());
	auto is_crc_correct = std::equal(crc_computed.begin(), crc_computed.end(), tag.data.begin() + 16);

	return is_crc_correct;
}

inline auto atqaReceived(interface::RFIDReader &rfidreader) -> bool
{
	if (!rfidreader.didTagCommunicationSucceed(ATQA_answer_size)) {
		return false;
	}

	const auto &tag = rfidreader.getTag();

	return checkATQA(tag);
}

inline auto registerReceived(interface::RFIDReader &rfidreader) -> bool
{
	if (!rfidreader.didTagCommunicationSucceed(register_answer_size)) {
		return false;
	}

	const auto &tag = rfidreader.getTag();

	return checkRegister(tag);
}

namespace sm::event {

	struct tag_response_available {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto idle				= boost::sml::state<class idle>;
	inline auto send_reqa			= boost::sml::state<class send_reqa>;
	inline auto requesting_atqa		= boost::sml::state<class requesting_atqa>;
	inline auto requesting_tag_data = boost::sml::state<class requesting_tag_data>;

}	// namespace sm::state

namespace sm::guard {

	using irfidreader = interface::RFIDReader;

	struct atqa_received {
		auto operator()(irfidreader &rfidreader) const { return atqaReceived(rfidreader); }
	};

	struct register_received {
		auto operator()(irfidreader &rfidreader) const { return registerReceived(rfidreader); }
	};

}	// namespace sm::guard

namespace sm::action {

	using irfidreader = interface::RFIDReader;

	struct set_mode_tag_detection {
		auto operator()(irfidreader &rfidreader) const { rfidreader.setModeTagDetection(); }
	};

	struct set_communication_protocol {
		auto operator()(irfidreader &rfidreader) const
		{
			rfidreader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
		}
	};

	struct send_request_A {
		auto operator()(irfidreader &rfidreader) const { rfidreader.sendRequestToTag(command_requestA.data()); }
	};

	struct send_register_4 {
		auto operator()(irfidreader &rfidreader) const { rfidreader.sendRequestToTag(command_read_register_4.data()); }
	};

	struct on_tag_readable {
		auto operator()(irfidreader &rfidreader) const { rfidreader.onTagReadable(); }
	};

}	// namespace sm::action

struct ISO14443A {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::idle  + event<sm::event::tag_response_available> / (sm::action::set_communication_protocol {})                   = sm::state::send_reqa

			, sm::state::send_reqa             + event<sm::event::tag_response_available>                                      / (sm::action::send_request_A  {})          = sm::state::requesting_atqa

			, sm::state::requesting_atqa       + event<sm::event::tag_response_available>  [sm::guard::atqa_received {}]       / (sm::action::send_register_4  {})         = sm::state::requesting_tag_data
			, sm::state::requesting_atqa       + event<sm::event::tag_response_available>  [!sm::guard::atqa_received {}]      / (sm::action::set_mode_tag_detection {})   = sm::state::idle

			, sm::state::requesting_tag_data   + event<sm::event::tag_response_available>  [sm::guard::register_received {}]   / (sm::action::on_tag_readable {})          = sm::state::idle
			, sm::state::requesting_tag_data   + event<sm::event::tag_response_available>  [!sm::guard::register_received {}]                                              = sm::state::idle

			, sm::state::requesting_tag_data   + boost::sml::on_exit<_> / ( sm::action::set_mode_tag_detection {})

			// clang-format on
		);
	}
};

}	// namespace leka::rfid
