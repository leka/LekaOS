// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "MagicCard.h"
#include "boost/sml.hpp"
#include "interface/libs/MagicCardKit.h"

namespace leka::rfid::magic_card {

namespace sm::event {
	struct activity_detected {
	};
	struct activity_stopped {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto idle		  = boost::sml::state<class idle>;
	inline auto play_activity = boost::sml::state<class play_activity>;

}	// namespace sm::state

namespace sm::action {

	using imck = interface::MagicCardKit;

	struct start_activity {
		auto operator()(imck &magic_card_kit) const { magic_card_kit.start(); }
	};

}	// namespace sm::action

struct StateMachineActivity {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			*  sm::state::idle  + event<sm::event::activity_detected>   / (sm::action::start_activity {})   = sm::state::play_activity

			, sm::state::play_activity + event<sm::event::activity_stopped>                                 = sm::state::idle
			// clang-format on
		);
	}
};

}	// namespace leka::rfid::magic_card
