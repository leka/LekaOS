// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "boost/sml.hpp"

namespace leka::system::robot {

namespace sm::event {

	struct setup_completed {
	};
	struct going_to_sleep {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto setup	 = boost::sml::state<class setup>;
	inline auto idle	 = boost::sml::state<class idle>;
	inline auto sleeping = boost::sml::state<class sleeping>;

}	// namespace sm::state

namespace sm::guard {

}	// namespace sm::guard

namespace sm::action {

}	// namespace sm::action

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off

			* sm::state::setup + event<sm::event::setup_completed> = sm::state::idle

			, sm::state::idle  + event<sm::event::going_to_sleep>  = sm::state::sleeping

			// clang-format on
		);
	}
};

}	// namespace leka::system::robot
