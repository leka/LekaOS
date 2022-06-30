// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "boost/sml.hpp"
#include "interface/drivers/RFIDReader.h"

namespace leka::rfid {

namespace sm::event {

	struct tag_detected {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto idle			= boost::sml::state<class idle>;
	inline auto requesting_atqa = boost::sml::state<class requesting_atqa>;
	inline auto identifying_tag = boost::sml::state<class identifying_tag>;
	inline auto validating_tag	= boost::sml::state<class validating_tag>;

}	// namespace sm::state

namespace sm::guard {

	using irfidkit	  = interface::RFIDReader::ISO14443;
	using irfidreader = interface::RFIDReader;

	struct is_atqa_received {
		auto operator()(irfidkit &rfidkit) const { return rfidkit.receiveATQA(); }
	};
	struct is_atqa_not_received {
		auto operator()(irfidkit &rfidkit) const { return !rfidkit.receiveATQA(); }
	};
	struct is_register_received {
		auto operator()(irfidkit &rfidkit) const { return rfidkit.receiveReadTagData(); }
	};
	struct is_register_not_received {
		auto operator()(irfidkit &rfidkit) const { return !rfidkit.receiveReadTagData(); }
	};
	struct is_tag_valid {
		auto operator()(irfidkit &rfidkit) const
		{
			return rfidkit.receiveReadTagData() && rfidkit.isTagSignatureValid();
		}
	};
	struct is_tag_not_valid {
		auto operator()(irfidkit &rfidkit) const
		{
			return !rfidkit.receiveReadTagData() || !rfidkit.isTagSignatureValid();
		}
	};
}	// namespace sm::guard

namespace sm::action {

	using irfidkit	  = interface::RFIDReader::ISO14443;
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
		auto operator()(irfidkit &rfidkit) const { rfidkit.sendREQA(); }
	};

	struct send_register_0 {
		auto operator()(irfidkit &rfidkit) const { rfidkit.sendReadRegister0(); }
	};

	struct send_register_4 {
		auto operator()(irfidkit &rfidkit) const { rfidkit.sendReadRegister4(); }
	};

	struct register_magic_card {
		auto operator()(irfidkit &rfidkit) const { rfidkit.registerMagicCard(); }
	};

}	// namespace sm::action

struct RFIDStateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::idle               + event<sm::event::tag_detected>         / (sm::action::set_communication_protocol {}, sm::action::send_request_A  {})  = sm::state::requesting_atqa

			, sm::state::requesting_atqa    [sm::guard::is_atqa_received {}]         / (sm::action::send_register_0  {})                                            = sm::state::identifying_tag
			, sm::state::requesting_atqa    [sm::guard::is_atqa_not_received {}]     / (sm::action::set_tag_detection_mode {})                                      = sm::state::idle

			, sm::state::identifying_tag    [sm::guard::is_register_received {}]     / (sm::action::send_register_4  {})                                            = sm::state::validating_tag
			, sm::state::identifying_tag    [sm::guard::is_register_not_received {}] / (sm::action::set_tag_detection_mode {})                                      = sm::state::idle

			, sm::state::validating_tag     [sm::guard::is_tag_valid {}]             / (sm::action::register_magic_card {}, sm::action::set_tag_detection_mode {})  = sm::state::idle
			, sm::state::validating_tag     [sm::guard::is_tag_not_valid {}]         / (sm::action::set_tag_detection_mode {})                                      = sm::state::idle

			// clang-format on
		);
	}
};

}	// namespace leka::rfid
