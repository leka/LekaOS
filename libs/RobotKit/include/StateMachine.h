// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
#define _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_

#include "boost/sml.hpp"
#include "interface/RobotController.h"

namespace leka::system::robot {

namespace sm::event {

	struct setup_complete {
	};
	struct charge_did_start {
	};
	struct charge_did_stop {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto setup	 = boost::sml::state<class setup>;
	inline auto idle	 = boost::sml::state<class idle>;
	inline auto charging = boost::sml::state<class charging>;

}	// namespace sm::state

namespace sm::guard {

	using irc = interface::RobotController;

	struct is_charging {
		auto operator()(irc &rc) const { return rc.isCharging(); }
	};

	struct is_not_charging {
		auto operator()(irc &rc) const { return !rc.isCharging(); }
	};

}	// namespace sm::guard

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::setup    + event<sm::event::setup_complete>                                   = sm::state::idle
			, sm::state::idle     + event<sm::event::charge_did_start> [sm::guard::is_charging {}]     = sm::state::charging
			, sm::state::charging + event<sm::event::charge_did_stop>  [sm::guard::is_not_charging {}] = sm::state::idle
			// clang-format on
		);
	}
};

}	// namespace leka::system::robot

#endif	 // _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
