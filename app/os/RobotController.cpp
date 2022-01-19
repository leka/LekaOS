
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
}

void RobotController::registerEvents()
{
	// Allow actions to be
	_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});

	// Initializations
	_leds_utils.initialize();
	_leds_utils.initializationAnimation();

	// Setup callbacks for each events
	mbed::Ticker t_timeout;
	t_timeout.attach({this, &RobotController::raise<sm::event::timeout>}, 20s);	  // OK

	rtos::ThisThread::sleep_for(10s);

	mbed::Ticker t_wakeup;
	t_wakeup.attach({this, &RobotController::raise<sm::event::wakeup>}, 20s);	// OK

	// Optional, only for objects that can vanished
	while (true) {
		rtos::ThisThread::sleep_for(1s);
	}
}

void RobotController::onEntrySleeping()
{
	log_info("On Entry Sleeping.");

	rtos::ThisThread::sleep_for(100ms);

	// _leds_utils.runReinforcer(LedsReinforcer::green);									 // NOK
	_event_queue.call(&_leds_utils, &LedsUtils::runReinforcer, LedsReinforcer::green);	 // OK
}

void RobotController::onEntryWaitingForCommands()
{
	log_info("On Entry Waiting For Commands.");

	rtos::ThisThread::sleep_for(100ms);

	_event_queue.call(&_leds_utils, &LedsUtils::runReinforcer, LedsReinforcer::fire);	// OK
}
