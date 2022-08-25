// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Game.h"
#include "boost/sml.hpp"

namespace leka::game {

namespace sm::event {
	struct start_game {
	};
	struct stop_game {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto idle		 = boost::sml::state<class idle>;
	inline auto playing_game = boost::sml::state<class playing_game>;

}	// namespace sm::state

struct SMAutonomousGame {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::idle         + event<sm::event::start_game>  = sm::state::playing_game
			, sm::state::playing_game + event<sm::event::stop_game>   = sm::state::idle
			// clang-format on
		);
	}
};

}	// namespace leka::game
