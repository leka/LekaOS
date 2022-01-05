// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
#define _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_

#include "boost/sml.hpp"
#include "interface/RobotController.h"

namespace leka::system::robot {

namespace sm::event {

	struct start {
	};
	struct timeout {
	};

}	// namespace sm::event

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		auto action_start_system = [](interface::RobotController &rc) { rc.startSystem(); };
		auto action_stop_system	 = [](interface::RobotController &rc) { rc.stopSystem(); };

		return make_transition_table(
			// clang-format off
			 * "idle"_s    + event<sm::event::start>   / action_start_system = "running"_s
			,  "running"_s + event<sm::event::timeout> / action_stop_system  = "idle"_s
			// clang-format on
		);
	}
};

}	// namespace leka::system::robot

#endif	 // _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
