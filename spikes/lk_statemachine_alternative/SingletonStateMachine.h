// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SINGLETON_STATE_MACHINE_
#define _LEKA_OS_SINGLETON_STATE_MACHINE_

#include <string>

#include "EventsStateMachine.h"

#include "EventFlags.h"

namespace leka {

class SingletonStateMachine
{
  public:
	SingletonStateMachine(rtos::EventFlags &event_flags) : _event_flags(event_flags) {};

	auto getState() -> std::string { return _state.getState(); }

	void StateLoop()
	{
		while (true) {
			_event_flags.wait_any(static_cast<uint32_t>(Events::ALL), osWaitForever, false);
			event = static_cast<Events>(_event_flags.get());
			_event_flags.clear(static_cast<uint32_t>(event));

			_state = _state.ProcessEvent(event);
		}
	};

  private:
	struct State {
		virtual auto getState() -> std::string { return "State"; };
		virtual auto ProcessEvent(Events event) -> State & = 0;
	};

	struct WaitingForCommands : State {
		WaitingForCommands(WaitingForCommands const &) = delete;
		void operator=(WaitingForCommands const &) = delete;

		static auto getInstance() -> WaitingForCommands &
		{
			static WaitingForCommands instance;
			return instance;
		}

		auto getState() -> std::string final { return "Waiting For Commands"; };

		auto ProcessEvent(Events event) -> State & final
		{
			switch (event) {
				case Events::TIMEOUT:
					// Turn off every actuators
					return Sleep::getInstance();
				default:
					break;
			}
			return getInstance();
		};

	  private:
		WaitingForCommands() = default;
	};

	struct Sleep : State {
		Sleep(Sleep const &) = delete;
		void operator=(Sleep const &) = delete;

		static auto getInstance() -> Sleep &
		{
			static Sleep instance;
			return instance;
		}

		auto getState() -> std::string final { return "Sleep"; };

		auto ProcessEvent(Events event) -> State & final
		{
			switch (event) {
				case Events::WAKEUP:
					// event [conditional] \ action
					// Turn on screen
					return WaitingForCommands::getInstance();
					break;
				default:
					break;
			}
			return getInstance();
		};

	  private:
		Sleep() = default;
	};

	Events event = Events::NONE;
	rtos::EventFlags &_event_flags;

	State &_state = WaitingForCommands::getInstance();
};

}	// namespace leka

#endif	 // _LEKA_OS_SINGLETON_STATE_MACHINE_
