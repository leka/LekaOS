// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_STATE_MACHINE_KIT_H_
#define _LEKA_OS_LIB_STATE_MACHINE_KIT_H_

#include "boost/sml.hpp"

namespace leka {

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

		return make_transition_table(
			// clang-format off
			 * "idle"_s    + event<sml::event::start>   = "running"_s
			,  "running"_s + event<sml::event::timeout> = "idle"_s
			// clang-format on
		);
	}
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_STATE_MACHINE_KIT_H_
