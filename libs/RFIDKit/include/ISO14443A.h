// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>

#include "boost/sml.hpp"
#include "interface/drivers/RFIDReader.h"

namespace leka::rfid {

template <size_t SIZE>
struct Command {
	const std::array<uint8_t, SIZE> data;
	const leka::rfid::Flag flags;

	[[nodiscard]] inline auto getArray() const -> std::array<uint8_t, SIZE + 1>
	{
		auto cmd = std::array<uint8_t, SIZE + 1> {};

		for (int i = 0; i < SIZE; ++i) {
			cmd.at(i) = data.at(i);
		}

		cmd.at(SIZE) = static_cast<uint8_t>(flags);

		return cmd;
	}
};

constexpr Command<1> command_requestA		 = {.data = {0x26}, .flags = leka::rfid::Flag::sb_7};
constexpr Command<2> command_read_register_4 = {.data  = {0x30, 0x04},
												.flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};

constexpr size_t ATQA_answer_size	  = 2;
constexpr size_t register_answer_size = 18;
constexpr std::array<uint8_t, ATQA_answer_size> expected_ATQA_answer {0x44, 0x00};

inline auto computeCRC(uint8_t const *data) -> std::array<uint8_t, 2>
{
	uint32_t wCrc = 0x6363;
	size_t size	  = 16;

	do {
		std::byte bt;
		bt	 = static_cast<std::byte>(*data++);
		bt	 = (bt ^ static_cast<std::byte>(wCrc & 0x00FF));
		bt	 = (bt ^ (bt << 4));
		wCrc = (wCrc >> 8) ^ (static_cast<uint32_t>(bt) << 8) ^ (static_cast<uint32_t>(bt) << 3) ^
			   (static_cast<uint32_t>(bt) >> 4);
	} while (--size);

	std::array<uint8_t, 2> pbtCrc = {static_cast<uint8_t>(wCrc & 0xFF), static_cast<uint8_t>((wCrc >> 8) & 0xFF)};
	return pbtCrc;
}

inline auto receiveAtqa(interface::RFIDReader &rfidreader) -> bool
{
	if (!rfidreader.didTagCommunicationSucceed(ATQA_answer_size)) {
		return false;
	}

	auto tag = rfidreader.getTag();

	auto is_atqa_correct = std::equal(expected_ATQA_answer.begin(), expected_ATQA_answer.end(), tag.data.begin());
	return is_atqa_correct;
}

inline auto receiveRegister(interface::RFIDReader &rfidreader) -> bool
{
	if (!rfidreader.didTagCommunicationSucceed(register_answer_size)) {
		return false;
	}
	auto tag = rfidreader.getTag();

	auto crc_computed	= computeCRC(tag.data.data());
	auto is_crc_correct = std::equal(crc_computed.begin(), crc_computed.end(), tag.data.begin() + 16);

	return is_crc_correct;
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
		auto operator()(irfidreader &rfidreader) const { return receiveAtqa(rfidreader); }
	};

	struct register_received {
		auto operator()(irfidreader &rfidreader) const { return receiveRegister(rfidreader); }
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
		auto operator()(irfidreader &rfidreader) const { rfidreader.sendRequestToTag(command_requestA.getArray()); }
	};

	struct send_register_4 {
		auto operator()(irfidreader &rfidreader) const
		{
			rfidreader.sendRequestToTag(command_read_register_4.getArray());
		}
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
