// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "boost/sml.hpp"
#include "interface/RobotController.h"

namespace leka::system::robot {

namespace sm::event {

	struct setup_complete {
	};
	struct sleep_timeout_did_end {
	};
	struct charge_did_start {
	};
	struct charge_did_stop {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto setup	 = boost::sml::state<class setup>;
	inline auto idle	 = boost::sml::state<class idle>;
	inline auto sleeping = boost::sml::state<class sleeping>;
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

namespace sm::action {

	using irc = interface::RobotController;

	struct run_launching_behavior {
		auto operator()(irc &rc) const { rc.runLaunchingBehavior(); }
	};

	struct start_sleep_timeout {
		auto operator()(irc &rc) const { rc.startSleepTimeout(); }
	};

	struct stop_sleep_timeout {
		auto operator()(irc &rc) const { rc.stopSleepTimeout(); }
	};

	struct start_sleeping_behavior {
		auto operator()(irc &rc) const { rc.startSleepingBehavior(); }
	};

	struct stop_sleeping_behavior {
		auto operator()(irc &rc) const { rc.stopSleepingBehavior(); }
	};

}	// namespace sm::action

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::setup    + event<sm::event::setup_complete>                                   = sm::state::idle
			, sm::state::setup    + boost::sml::on_exit<_>  / sm::action::run_launching_behavior {}

			, sm::state::idle     + boost::sml::on_entry<_> / sm::action::start_sleep_timeout {}
			, sm::state::idle     + boost::sml::on_exit<_>  / sm::action::stop_sleep_timeout  {}

			, sm::state::idle     + event<sm::event::sleep_timeout_did_end>                            = sm::state::sleeping
			, sm::state::idle     + event<sm::event::charge_did_start> [sm::guard::is_charging {}]     = sm::state::charging

			, sm::state::sleeping + boost::sml::on_entry<_> / sm::action::start_sleeping_behavior {}
			, sm::state::sleeping + boost::sml::on_exit<_>  / sm::action::stop_sleeping_behavior {}

			, sm::state::sleeping + event<sm::event::charge_did_start> [sm::guard::is_charging {}]     = sm::state::charging

			, sm::state::charging + event<sm::event::charge_did_stop>  [sm::guard::is_not_charging {}] = sm::state::idle
			// clang-format on
		);
	}
};

}	// namespace leka::system::robot
