// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
#define _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_

#include "boost/sml.hpp"
#include "interface/RobotController.h"

namespace leka::system::robot {

namespace sm::event {

	struct wakeup {
	};
	struct timeout {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto sleeping			 = boost::sml::state<class sleeping>;
	inline auto waiting_for_commands = boost::sml::state<class waiting_for_commands>;

}	// namespace sm::state

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		auto action_wakeup_system	   = [](interface::RobotController &rc) { rc.wakeupSystem(); };
		auto action_fall_asleep_system = [](interface::RobotController &rc) { rc.fallAsleepSystem(); };

		auto on_entry_waiting_for_commands = [](interface::RobotController &rc) { rc.onEntryWaitingForCommands(); };
		auto on_entry_sleeping			   = [](interface::RobotController &rc) { rc.onEntrySleeping(); };

		return make_transition_table(
			// clang-format off
			 * sm::state::waiting_for_commands + boost::sml::on_entry<_>   / on_entry_waiting_for_commands
			, sm::state::waiting_for_commands  + event<sm::event::timeout> / action_fall_asleep_system  = sm::state::sleeping
			, sm::state::sleeping              + boost::sml::on_entry<_>   / on_entry_sleeping
			, sm::state::sleeping              + event<sm::event::wakeup>  / action_wakeup_system       = sm::state::waiting_for_commands
			// clang-format on
		);
	}
};

}	// namespace leka::system::robot

#endif	 // _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
