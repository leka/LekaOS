
#include "RobotController.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;
using namespace system::robot;

void RobotController::setStateMachine(boost::sml::sm<StateMachine> *sm)
{
	_sm = sm;
}

void RobotController::startEventQueueDispatch()
{
	_thread_event_queue.start({&_event_queue, &events::EventQueue::dispatch_forever});
}

void RobotController::startMainLoop()
{
	if (_sm == nullptr) {
		return;
	}

	auto timeout_flag_is_set = [&]() {
		return (_event_flags.get() & RobotController::Flags::TIMEOUT) == RobotController::Flags::TIMEOUT;
	};

	auto wakeup_flag_is_set = [&]() {
		return (_event_flags.get() & RobotController::Flags::WAKEUP) == RobotController::Flags::WAKEUP;
	};

	startEventQueueDispatch();
	_sm->process_event(sm::event::wakeup {});

	int id = 0;

	while (true) {
		_event_flags.wait_any(RobotController::Flags::ALL, osWaitForever, false);

		while (!process_id.empty()) {
			process_id.pop(id);
			_event_queue.cancel(id);
		}

		if (timeout_flag_is_set()) {
			_sm->process_event(sm::event::timeout {});

		} else if (wakeup_flag_is_set()) {
			_sm->process_event(sm::event::wakeup {});
		}

		_event_flags.clear(RobotController::Flags::ALL);
	}
}

void RobotController::wakeupSystem()
{
	log_info("Wakeup System.");
}

void RobotController::fallAsleepSystem()
{
	log_info("Fall asleep System.");

	process_id.push(
		_event_queue.call_in(1s, &_event_flags, &rtos::EventFlags::set, static_cast<uint32_t>(Flags::WAKEUP)));
}

void RobotController::onEntryWaitingForCommands()
{
	log_info("On Entry Waiting For Commands.");

	// METHOD 1
	// Run processes
	// Wait any event flag
	// Propagate event in StateMachine
	// Cancel/Kill all processes
	// Clear events flag

	// METHOD 2
	// Run processes
	// Let main loop handle the rest

	process_id.push(
		_event_queue.call_in(5s, &_event_flags, &rtos::EventFlags::set, static_cast<uint32_t>(Flags::TIMEOUT)));
}
