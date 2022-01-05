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

class Robot
{
  public:
	Robot(rtos::EventFlags &event_flags) : _event_flags(event_flags) {};

	void StateLoop()
	{
		_event_flags.wait_any(static_cast<uint32_t>(Events::ALL), osWaitForever, false);
		event = static_cast<Events>(_event_flags.get());
		_event_flags.clear(static_cast<uint32_t>(event));

		_state = std::visit([](auto &&state) -> State { return state.ProcessEvent(); }, _state);
	};

  private:
	struct State {
		virtual auto ProcessEvent() -> State { return State(); };
	};

	struct WaitingForCommands : State {
		auto ProcessEvent() -> State final
		{
			switch (event) {
				case Events::TIMEOUT:
					// Turn off every actuators
					return Sleep();
					break;
				default:
					break;
			}
			return WaitingForCommands();
		};
	};

	struct Sleep : State {
		auto ProcessEvent() -> State final
		{
			switch (event) {
				case Events::WAKEUP:
					// event [conditional] \ action
					// Turn on screen
					return WaitingForCommands();
					break;
				default:
					break;
			}
			return Sleep();
		};
	};

	inline static Events event = Events::NONE;
	rtos::EventFlags &_event_flags;

	std::variant<State, WaitingForCommands, Sleep> _state = WaitingForCommands();
};

}	// namespace leka

#endif	 // _LEKA_OS_STATE_MACHINE_
