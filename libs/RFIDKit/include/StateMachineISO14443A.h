// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LogKit.h"
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
			cmd[i] = data[i];
		}

		cmd[SIZE] = static_cast<uint8_t>(flags);

		return cmd;
	}
};

Command<1> command_requestA		   = {.data = {0x26}, .flags = leka::rfid::Flag::sb_7};
Command<2> command_read_register_0 = {.data = {0x30, 0x00}, .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};
Command<2> command_read_register_4 = {.data = {0x30, 0x04}, .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};
Command<2> command_read_register_6 = {.data = {0x30, 0x06}, .flags = leka::rfid::Flag::crc | leka::rfid::Flag::sb_8};

std::array<uint8_t, 2> ATQA_answer {0x44, 0x00};

namespace sm::event {

	struct registering_complete {
	};
	struct timeout_did_end {
	};
	struct tag_detected {
	};
	struct atqa_received {
	};
	struct atqa_not_received {
	};
	struct register_received {
	};
	struct register_not_received {
	};
	struct tag_valid {
	};
	struct tag_invalid {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto idle			= boost::sml::state<class idle>;
	inline auto requesting_atqa = boost::sml::state<class requesting_atqa>;
	inline auto identifying_tag = boost::sml::state<class identifying_tag>;
	inline auto validating_tag	= boost::sml::state<class validating_tag>;

}	// namespace sm::state

namespace sm::guard {
	using irfidreader = interface::RFIDReader;

	struct is_atqa_received {
		auto operator()(irfidreader &rfidreader) const { return rfidreader.receiveATQA(); }
	};
	struct is_atqa_not_received {
		auto operator()(irfidreader &rfidreader) const { return !rfidreader.receiveATQA(); }
	};
	struct is_register_received {
		auto operator()(irfidreader &rfidreader) const { return rfidreader.receiveReadTagData(); }
	};
	struct is_register_not_received {
		auto operator()(irfidreader &rfidreader) const { return !rfidreader.receiveReadTagData(); }
	};
	struct is_tag_valid {
		auto operator()(irfidreader &rfidreader) const
		{
			return rfidreader.receiveReadTagData() && rfidreader.isTagSignatureValid();
		}
	};
	struct is_tag_not_valid {
		auto operator()(irfidreader &rfidreader) const
		{
			return !rfidreader.receiveReadTagData() || !rfidreader.isTagSignatureValid();
		}
	};
}	// namespace sm::guard

namespace sm::action {

	using irfidreader = interface::RFIDReader;

	struct set_tag_detection_mode {
		auto operator()(irfidreader &rfidreader) const { rfidreader.setTagDetectionMode(); }
	};

	struct set_communication_protocol {
		auto operator()(irfidreader &rfidreader) const
		{
			rfidreader.setCommunicationProtocol(rfid::Protocol::ISO14443A);
		}
	};

	struct send_request_A {
		auto operator()(irfidreader &rfidreader) const { rfidreader.sendCommandToTag(command_requestA.getArray()); }
	};

	struct receiving_atqa {
		auto operator()(irfidreader &rfidreader) const { rfidreader.receiveATQA(); }
	};

	struct send_register_0 {
		auto operator()(irfidreader &rfidreader) const
		{
			rfidreader.sendCommandToTag(command_read_register_0.getArray());
		}
	};

	struct send_register_4 {
		auto operator()(irfidreader &rfidreader) const
		{
			rfidreader.sendCommandToTag(command_read_register_4.getArray());
		}
	};

	struct receiving_register {
		auto operator()(irfidreader &rfidreader) const { rfidreader.receiveReadTagData(); }
	};

	struct register_magic_card {
		auto operator()(irfidreader &rfidreader) const { rfidreader.onTagValid(); }
	};

}	// namespace sm::action

struct StateMachineISO14443A {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::idle               + event<sm::event::tag_detected>   / (sm::action::set_communication_protocol {}, sm::action::send_request_A {})  = sm::state::requesting_atqa

			, sm::state::requesting_atqa + boost::sml::on_entry<_> / ( sm::action::receiving_atqa {})

			, sm::state::requesting_atqa    + event<sm::event::atqa_received>      / (sm::action::send_register_0  {})                                       = sm::state::identifying_tag
			, sm::state::requesting_atqa    + event<sm::event::atqa_not_received>  / (sm::action::set_tag_detection_mode {})                                 = sm::state::idle

			, sm::state::identifying_tag + boost::sml::on_entry<_> / ( sm::action::receiving_register {})

			, sm::state::identifying_tag    + event<sm::event::register_received>     / (sm::action::send_register_4  {})                                    = sm::state::validating_tag
			, sm::state::identifying_tag    + event<sm::event::register_not_received> / (sm::action::set_tag_detection_mode {})                              = sm::state::idle

			, sm::state::validating_tag + boost::sml::on_entry<_> / ( sm::action::receiving_register {}, sm::action::register_magic_card {})

			, sm::state::validating_tag     + event<sm::event::tag_valid>     / (sm::action::register_magic_card {}, sm::action::set_tag_detection_mode {})  = sm::state::idle
			, sm::state::validating_tag     + event<sm::event::tag_invalid>   / (sm::action::set_tag_detection_mode {})                                      = sm::state::idle

			// clang-format on
		);
	}
};

}	// namespace leka::rfid
