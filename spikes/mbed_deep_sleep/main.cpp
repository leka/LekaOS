// mbed Microcontroller Library
// Copyright (c) 2019 ARM Limited
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>

#include "drivers/Watchdog.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_stats.h"
#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace std::chrono;

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
				"dt: %i, kck: %u, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
				"%u], hur: %u%% (%+i)[%u/%u]",
				delta, kick_count, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta, stack_used_size,
				stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size, heap_reserved_size);

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

auto main() -> int
{
	leka::logger::init();
	watchdog::start();

	auto allowed = sleep_manager_can_deep_sleep() ? 1 : 0;
	log_info("Deep sleep allowed: %d", allowed);

	while (true) {
		// ? This part is taken from mbed's documentation
		// ? sleep_manager_can_deep_sleep doesn't seem to work as expected
		// ? as it always return the same value (not allowed)
		// ? The statistics are good though as we see that sleep/deep sleep ratio
		// ? reach a 50/50 ratio as time passes by...

		// Deep sleep for 1 second
		allowed = sleep_manager_can_deep_sleep() ? 1 : 0;
		log_info("Deep sleep allowed: %d", allowed);
		rtos::ThisThread::sleep_for(1000ms);

		// Lock deep sleep
		log_info("Locking deep sleep");
		sleep_manager_lock_deep_sleep();

		// Sleep for 1 second
		allowed = sleep_manager_can_deep_sleep() ? 1 : 0;
		log_info("Deep sleep allowed: %d", allowed);
		rtos::ThisThread::sleep_for(1000ms);

		// Unlock deep sleep
		log_info("Unlocking deep sleep");
		sleep_manager_unlock_deep_sleep();

		// ? As an alternative, only keeping the following line
		// ? will make the deep sleep ratio reach 99%
		// rtos::ThisThread::sleep_for(1000ms);
	}
}
