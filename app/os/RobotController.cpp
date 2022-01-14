
#include "RobotController.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;

void RobotController::startEventQueueDispatch()
{
	_thread_event_queue.start({&_event_queue, &events::EventQueue::dispatch_forever});
}

void RobotController::startSystem()
{
	log_info("Start System.");
}

void RobotController::stopSystem()
{
	log_info("Stop System.");

	_event_queue.call_in(1s, &_event_flags, &rtos::EventFlags::set, static_cast<uint32_t>(Flags::START));
}

void RobotController::onRunningEntry()
{
	log_info("On Running Entry.");

	// METHOD 1
	// Run processes
	// Wait any event flag
	// Propagate event in StateMachine
	// Cancel/Kill all processes
	// Clear events flag

	// METHOD 2
	// Run processes
	// Let main loop handle the rest

	_event_queue.call_in(5s, &_event_flags, &rtos::EventFlags::set, static_cast<uint32_t>(Flags::TIMEOUT));
}
