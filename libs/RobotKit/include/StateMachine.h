// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
#define _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_

#include "boost/sml.hpp"

namespace leka {

namespace interface {

	class StateMachineManager
	{
	  public:
		virtual ~StateMachineManager() = default;

		virtual void startSystem() = 0;
		virtual void stopSystem()  = 0;
	};

}	// namespace interface

namespace sml::event {

	struct start {
	};
	struct timeout {
	};

}	// namespace sml::event

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		auto action_start_system = [](interface::StateMachineManager &manager) { manager.startSystem(); };
		auto action_stop_system	 = [](interface::StateMachineManager &manager) { manager.stopSystem(); };

		return make_transition_table(
			// clang-format off
			 * "idle"_s    + event<sml::event::start>   / action_start_system = "running"_s
			,  "running"_s + event<sml::event::timeout> / action_stop_system  = "idle"_s
			// clang-format on
		);
	}
};
}	// namespace leka

#endif	 // _LEKA_OS_LIB_ROBOT_KIT_STATE_MACHINE_H_
