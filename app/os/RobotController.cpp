
#include "RobotController.h"

#include "rtos/ThisThread.h"

#include "LogKit.h"
#include "Ticker.h"

using namespace leka;
using namespace std::chrono_literals;
using namespace system::robot;

template <typename T>
void RobotController::raise()
{
	_sm.process_event(T {});
}

void RobotController::wakeupSystem()
{
	log_info("Wakeup System.");
}

void RobotController::fallAsleepSystem()
{
	log_info("Fall asleep System.");

	rtos::ThisThread::sleep_for(100ms);

	// _sm.process_event(sm::event::wakeup {});

	// raise<sm::event::wakeup>();

	// auto lambda = [&]() { _sm.process_event(sm::event::wakeup {}); };
	// lambda();

	// auto lambda = [&]<typename T>(T event) { raise<T>(); };
	// lambda(sm::event::wakeup {});

	// mbed::Callback<void()> c {this, &RobotController::raise<sm::event::wakeup>};
	// c.call();
}

void RobotController::onEntryWaitingForCommands()
{
	log_info("On Entry Waiting For Commands.");

	rtos::ThisThread::sleep_for(100ms);

	// _sm.process_event(sm::event::timeout {});	// OK - Overflow

	// raise<sm::event::timeout>();   // OK - Overflow

	// _event_queue.call(this, &RobotController::raise<sm::event::timeout>);	// NOK - Hardfault

	// const auto e = sm::event::timeout {};
	// _event_queue.call(&_sm, &boost::sml::sm<StateMachine>::process_event<sm::event::timeout, 0>,
	// 				  e);	// Compilation NOK

	// mbed::Ticker t;
	// t.attach({this, &RobotController::raise<sm::event::timeout>}, 100ms);	// NOK - Hardfault

	// mbed::Callback<void()> c {this, &RobotController::raise<sm::event::timeout>};
	// c.call();				// OK - Overflow
	// _event_queue.call(c);	// NOK - Hardfault
	// t.attach(c, 100ms);		// NOK - Hardfault

	// auto lambda = [&]() { _sm.process_event(sm::event::timeout {}); };
	// lambda();					 // OK - Overflow
	// _event_queue.call(lambda);	 // NOK - Hardfault

	// auto lambda = [&]<typename T>(T event) { raise<T>(); };
	// lambda(sm::event::timeout {});						// OK - Overflow
	// _event_queue.call(lambda, sm::event::timeout {});	// NOK - Hardfault
}
