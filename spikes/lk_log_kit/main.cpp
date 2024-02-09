// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "drivers/Ticker.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreEventQueue.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "include/utils.h"

using namespace leka;
using namespace mbed;
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

auto hello = HelloWorld {};

auto thread_log_debug  = rtos::Thread {};
auto thread_log_printf = rtos::Thread {};

auto ticker_log_from_isr = Ticker {};
auto event_queue		 = CoreEventQueue {};

auto main() -> int
{
	logger::init();
	event_queue.dispatch_forever();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, Investigation Day!\n");

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	// ? You can un/comment the following lines to filter console output
	thread_log_debug.start(log_thread_debug);
	// rtos::ThisThread::sleep_for(1s);
	// thread_log_printf.start(log_thread_printf);

	auto log_isr_lambda = [] {
		// ? After testing, it turns out that LogKit cannot be easily used from ISR
		// ? This is because it uses snprintf to format the output which is not ISR compatible
		// ? One can test the faulty behavior here by un/commenting the corresponding line

		// ? w/ event queue
		event_queue.call(&log_from_isr);
		// ? w/o event queue
		// log_from_isr();
	};

	// ticker_log_from_isr.attach(log_isr_lambda, 5s);

	{
		auto start = rtos::Kernel::Clock::now();
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		auto stop = rtos::Kernel::Clock::now();

		log_info("log_debug took %i ms to complete... that's fast!\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(3333ms);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
