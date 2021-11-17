#include "WatchdogUtils.h"

#include "drivers/Watchdog.h"
#include "rtos/ThisThread.h"

using namespace std::chrono_literals;

auto &watchdog		   = mbed::Watchdog::get_instance();
constexpr auto timeout = 5000ms;

void startWatchdog(rtos::Thread &thread_watchdog)
{
	watchdog.start(timeout.count());
	thread_watchdog.start(watchdogLoop);
}

void watchdogLoop()
{
	while (true) {
		watchdog.kick();
		rtos::ThisThread::sleep_for(timeout - 1s);
	}
}
