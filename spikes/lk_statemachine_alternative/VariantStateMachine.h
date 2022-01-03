// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_VARIANT_STATE_MACHINE_
#define _LEKA_OS_VARIANT_STATE_MACHINE_

#include <string>
#include <variant>

#include "EventsStateMachine.h"

#include "EventFlags.h"

namespace leka {

class VariantStateMachine
{
  public:
	VariantStateMachine(rtos::EventFlags &event_flags) : _event_flags(event_flags) {};

	auto getState() -> std::string
	{
		return std::visit([&](auto &&state) -> std::string { return state.getState(); }, _state);
	}

	void StateLoop()
	{
		while (true) {
			_event_flags.wait_any(static_cast<uint32_t>(Events::ALL), osWaitForever, false);
			event = static_cast<Events>(_event_flags.get());
			_event_flags.clear(static_cast<uint32_t>(event));

			_state = std::visit([&](auto &&state) -> State { return state.ProcessEvent(event); }, _state);
		}
	};

  private:
	struct State {
		virtual auto getState() -> std::string { return "State"; };
		virtual auto ProcessEvent(Events event) -> State { return State(); };
	};

	struct WaitingForCommands : State {
		auto getState() -> std::string final { return "Waiting For Commands"; };

		auto ProcessEvent(Events event) -> State final
		{
			switch (event) {
				case Events::TIMEOUT:
					// Turn off every actuators
					return Sleep();
				default:
					break;
			}
			return WaitingForCommands();
		};
	};

	struct Sleep : State {
		auto getState() -> std::string final { return "Sleep"; };

		auto ProcessEvent(Events event) -> State final
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

	Events event = Events::NONE;
	rtos::EventFlags &_event_flags;

	std::variant<State, WaitingForCommands, Sleep> _state = WaitingForCommands();
};

}	// namespace leka

#endif	 // _LEKA_OS_VARIANT_STATE_MACHINE_
