// Leka - LekaOS
// Copyright (c) 2020 Arm Limited and affiliates.
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"
#include "drivers/Ticker.h"
#include "drivers/Watchdog.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

#include "mbed_stats.h"

#include "drivers/Watchdog.h"

namespace {

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 30000ms;
		auto thread			   = rtos::Thread {osPriorityLow};

		namespace stats {

			auto cpu   = mbed_stats_cpu_t {};
			auto stack = mbed_stats_stack_t {};
			auto heap  = mbed_stats_heap_t {};

		}	// namespace stats

		__attribute__((noreturn)) void watchdog_kick()
		{
			static auto kick_count = uint32_t {0};

			static auto start = rtos::Kernel::Clock::now();
			static auto stop  = rtos::Kernel::Clock::now();
			static auto delta = static_cast<int>((stop - start).count());

			static auto idle_ratio		 = uint8_t {};
			static auto sleep_ratio		 = uint8_t {};
			static auto deep_sleep_ratio = uint8_t {};

			static auto stack_used_delta	= int32_t {};
			static auto stack_used_size		= uint32_t {};
			static auto stack_reserved_size = uint32_t {};
			static auto stack_used_ratio	= uint8_t {};

			static auto heap_used_delta	   = int32_t {};
			static auto heap_used_size	   = uint32_t {};
			static auto heap_reserved_size = uint32_t {};
			static auto heap_used_ratio	   = uint8_t {};

			while (true) {
				internal::instance.kick();
				++kick_count;

				stop  = rtos::Kernel::Clock::now();
				delta = static_cast<int>((stop - start).count());

				mbed_stats_cpu_get(&stats::cpu);

				idle_ratio	= static_cast<uint8_t>(((stats::cpu.idle_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				sleep_ratio = static_cast<uint8_t>(((stats::cpu.sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				deep_sleep_ratio =
					static_cast<uint8_t>(((stats::cpu.deep_sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));

				mbed_stats_stack_get(&stats::stack);

				stack_used_delta	= static_cast<int32_t>(stats::stack.max_size - stack_used_size);
				stack_used_size		= stats::stack.max_size;
				stack_reserved_size = stats::stack.reserved_size;
				stack_used_ratio	= static_cast<uint8_t>((stack_used_size * 100) / stack_reserved_size);

				mbed_stats_heap_get(&stats::heap);

				heap_used_delta	   = static_cast<int32_t>(stats::heap.current_size - heap_used_size);
				heap_used_size	   = stats::heap.current_size;
				heap_reserved_size = stats::heap.reserved_size;
				heap_used_ratio	   = static_cast<uint8_t>((heap_used_size * 100) / heap_reserved_size);

				log_info(
					"dt: %i, kck: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, idle_ratio, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta,
					stack_used_size, stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size,
					heap_reserved_size);

				start = rtos::Kernel::Clock::now();
				rtos::ThisThread::sleep_for(5s);
			}
		}

	}	// namespace internal

	void start()
	{
		internal::instance.start(internal::timeout.count());
		internal::thread.start(watchdog::internal::watchdog_kick);
	}

}	// namespace watchdog

}	// namespace

///
/// @brief Uncomment the line you want to test. `USE_THREAD` will run the example with
/// a thread, whereas `USE_TICKER` will run the example with a ticker.
///

#define USE_THREAD
// #define USE_TICKER

//
// MARK:- Variables
//

struct variables {
	static inline auto led_main		= mbed::DigitalOut {LED1};
	static inline auto led_error	= mbed::DigitalOut {LED3};
	static inline auto led_watchdog = mbed::DigitalOut {LED2};

	static inline auto button = mbed::InterruptIn {USER_BUTTON};

	static inline auto &watchdog = mbed::Watchdog::get_instance();

	static inline auto thread_watchdog = rtos::Thread {};
	static inline auto ticker_watchdog = mbed::Ticker {};

	static inline auto should_abort		= false;
	static inline auto should_get_stuck = false;
};

//
// MARK:- Functions
//

void kickWatchdog()
{
	variables::watchdog.kick();
	variables::led_watchdog = !variables::led_watchdog;
}

// Abort or run infinite loop
void buttonInterrupt()
{
	// Comment the line you don't need
	// variables::should_abort = true;
	variables::should_get_stuck = true;
}

//
// MARK:- Thread
//

#ifdef USE_THREAD

void watchdogThreadFunction()
{
	while (true) {
		kickWatchdog();
		rtos::ThisThread::sleep_for(513ms);
	}
}

void initWatchdog()
{
	variables::watchdog.start(5000);
	variables::thread_watchdog.start(watchdogThreadFunction);
	variables::thread_watchdog.set_priority(osPriorityLow);
}

#endif

//
// MARK:- Ticker
//

#ifdef USE_TICKER

void initWatchdog()
{
	variables::watchdog.start(5000);
	variables::ticker_watchdog.attach(kickWatchdog, 500ms);
}

#endif

//
// MARK:- Main thread
//

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n\n");

	variables::led_error = 0;

	variables::led_main		= 1;
	variables::led_watchdog = 1;

	rtos::ThisThread::sleep_for(1s);

	variables::led_main		= 0;
	variables::led_watchdog = 0;

	rtos::ThisThread::sleep_for(1s);

	variables::led_main		= 1;
	variables::led_watchdog = 1;

	rtos::ThisThread::sleep_for(1s);

	variables::led_main		= 0;
	variables::led_watchdog = 0;

	rtos::ThisThread::sleep_for(1s);

	initWatchdog();

	variables::button.rise(&buttonInterrupt);

	{
		variables::led_main = !variables::led_main;

		rtos::ThisThread::sleep_for(100ms);

		if (variables::should_abort) {
			variables::led_error = !variables::led_error;
			abort();
		}

		if (variables::should_get_stuck) {
			while (true) {
				variables::led_error = !variables::led_error;
				// Comment the type of delay you want to test
				HAL_Delay(200);
				// rtos::ThisThread::sleep_for(200ms);
			}
		}
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
