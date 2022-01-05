// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_STATE_MACHINE_
#define _LEKA_OS_STATE_MACHINE_

#include <cstdint>
#include <variant>

#include "EventFlags.h"

namespace leka {

enum class Events : uint32_t
{
	NONE	= 0,
	WAKEUP	= (1UL << 0),
	TIMEOUT = (1UL << 1),
	ALL		= 0xFFFFFFFF
};

class RobotState
{
  public:
	RobotState(rtos::EventFlags &event_flags) : _event_flags(event_flags) {};

	void StateLoop();

  private:
	struct State {
		virtual auto processEvent(Events event) -> State { return State(); };
	};

	struct WaitingForCommands : State {
		auto processEvent(Events event) -> State final;
	};

	struct Sleep : State {
		auto processEvent(Events event) -> State final;
	};

	Events event = Events::NONE;
	rtos::EventFlags &_event_flags;

	std::variant<State, WaitingForCommands, Sleep> _state = WaitingForCommands();
};

}	// namespace leka

#endif	 // _LEKA_OS_STATE_MACHINE_
