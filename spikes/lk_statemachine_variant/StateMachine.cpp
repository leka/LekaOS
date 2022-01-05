#include "StateMachine.h"

using namespace leka;

auto WaitingForCommands::processEvent(Events event) -> interface::State {
	switch (event) {
		case Events::TIMEOUT:
			// Turn off every actuators
			return Sleep();
			break;
		default:
			break;
	}
	return WaitingForCommands();
}

auto Sleep::processEvent(Events event) -> interface::State {
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
}

void RobotState::StateLoop()
{
	// wait any flag
	event_flags.wait_any(static_cast<uint32_t>(Events::ALL));
	event = Events::WAKEUP;

	// _state = std::visit([](auto &&state) -> interface::State { return state.processEvent(Events::WAKEUP); }, _state);
	_state = std::visit([&](auto &&state) -> interface::State { return state.processEvent(event); }, _state);
}
