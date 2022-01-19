
#include "RobotController.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;
using namespace system::robot;

void nothing()
{
	log_info("Hello!");
	return;
}

// RobotController::RobotController()
// {
// 	// a = {this, &RobotController::process<sm::event::timeout>};
// }

void RobotController::start()
{
	t.start({&eq, &events::EventQueue::dispatch_forever});

	// eq.call_in(5s, this, &RobotController::processTimeout);
}

// void RobotController::setCallbacks()
// {
// 	// auto low_battery_callback = [&]() { _sm.process_event(sm::event::timeout {}); };

// 	// battery.setLowBatteryCallback(low_battery_callback);
// }

void RobotController::wakeupSystem()
{
	log_info("Wakeup System.");
}

void RobotController::fallAsleepSystem()
{
	log_info("Fall asleep System.");

	// auto lambda = [&]<typename eventToProcess>(eventToProcess event) { _sm.process_event(event); };

	// timeout.attach({this, &RobotController::processEventTimeout}, 5s);
	// timeout.attach({lambda, sm::event::wakeup {}}, 1s);
}

// template <typename eventToProcess>
// void processEvent(boost::sml::sm<StateMachine> sm, eventToProcess event)
// {
// 	sm.process_event(event);
// }

// void RobotController::processTimeout()
// {
// 	_sm.process_event(sm::event::timeout {});
// }

// void RobotController::process(sm::event::timeout e)
// {
// 	_sm.process_event(e);
// }

template <typename T>
void RobotController::process()
{
	_sm.process_event(T {});
}

void RobotController::onEntryWaitingForCommands()
{
	log_info("On Entry Waiting For Commands.");

	// timeout.registerCallback(&nothing);
	// log_info("Registered.");

	// auto event = boost::sml::event<sm::event::timeout>;

	// _lambda = [&]<typename eventToProcess>(eventToProcess event) { _sm.process_event(event); };

	// _lambda(sm::event::timeout {});
	// _lambda()
	// processEvent(sm::event::timeout {});

	// mbed::Callback<void()> a {nothing};
	// mbed::Callback<void()> b {this, &RobotController::processTimeout};
	// mbed::Callback<RobotController> c {&RobotController::process, sm::event::timeout {}};

	// timeout.attach({this, &RobotController::processEventTimeout}, 5s);
	// timeout.attach({this, &processEvent, sm::event::timeout {}}, 5s);
	// timeout.attach(b, 5s);

	// auto lambda = [&]<typename eventToProcess>(eventToProcess event) { _sm.process_event(event); };

	// auto now = timeout.scheduled_time();

	// timeout.attach_absolute(lambda, now + 5s);

	// process<sm::event::timeout>();

	// auto aaa = boost::sml::event<system::robot::sm::event::timeout>;

	// a = {this, &RobotController::process<sm::event::timeout>};
	// a();
	// process<sm::event::timeout>();
	// timeout.attach({this, &RobotController::process<sm::event::timeout>}, 5s);
	// auto l = [&]() { processTimeout(); };
	// timeout.attach(mbed::callback(this, &RobotController::processTimeout), 5s);

	// timeout.attach({nothing}, 3000ms);
	// timeout.attach(mbed::callback(this, &RobotController::processTimeout), 5s);
	// timeout.attach({nothing}, 4s);

	// eq.call_in(5s, this, &RobotController::processTimeout);
	// eq.call_in(5s, this, &RobotController::process<sm::event::timeout>);
	eq.call_in(5s, nothing);
	// _sm.process_event(sm::event::timeout {});
}
