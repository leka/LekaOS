// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_POINTER_STATE_MACHINE_
#define _LEKA_OS_POINTER_STATE_MACHINE_

#include <memory>
#include <string>

#include "EventsStateMachine.h"

#include "EventFlags.h"

namespace leka {

class PointerStateMachine
{
  public:
	PointerStateMachine(rtos::EventFlags &event_flags) : _event_flags(event_flags) {};

	auto getState() -> std::string { return _state->getState(); }

	void StateLoop()
	{
		while (true) {
			_event_flags.wait_any(static_cast<uint32_t>(Events::ALL), osWaitForever, false);
			event = static_cast<Events>(_event_flags.get());
			_event_flags.clear(static_cast<uint32_t>(event));

			_state = _state->ProcessEvent(event);
		}
	};

  private:
	struct State {
		virtual auto getState() -> std::string { return "State"; };
		virtual auto ProcessEvent(Events event) -> std::unique_ptr<State> = 0;
	};

	struct WaitingForCommands : State {
		auto getState() -> std::string final { return "Waiting For Commands"; };

		auto ProcessEvent(Events event) -> std::unique_ptr<State> final
		{
			std::unique_ptr<State> state;
			switch (event) {
				case Events::TIMEOUT:
					// Turn off every actuators
					state = std::make_unique<Sleep>();
					break;
				default:
					state = std::make_unique<WaitingForCommands>();
					break;
			}
			return state;
		};
	};

	struct Sleep : State {
		auto getState() -> std::string final { return "Sleep"; };

		auto ProcessEvent(Events event) -> std::unique_ptr<State> final
		{
			std::unique_ptr<State> state;
			switch (event) {
				case Events::WAKEUP:
					// event [conditional] \ action
					// Turn on screen
					state = std::make_unique<WaitingForCommands>();
					break;
				default:
					state = std::make_unique<Sleep>();
					break;
			}
			return state;
		};
	};

	Events event = Events::NONE;
	rtos::EventFlags &_event_flags;

	std::unique_ptr<State> _state;
};

}	// namespace leka

#endif	 // _LEKA_OS_POINTER_STATE_MACHINE_
