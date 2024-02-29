// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <span>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
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

void foo(std::span<int> span)
{
	log_info("from foo span size: %i", span.size());
	for (const auto &v: span) {
		log_info("v: %i", v);
	}
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	//
	// MARK: - std::to_array
	//

	log_info("Create array with std::to_array");

	auto arr = std::to_array({1, 2, 3, 4});

	for (const auto &v: arr) {
		log_info("v: %i", v);
	}

	//
	// MARK: - std::span
	//

	log_info("Pass std::array to std::span");

	foo(arr);

	log_info("Create span from array, auto deduce size");

	auto span0 = std::span {arr};

	for (const auto &v: span0) {
		log_info("v: %i", v);
	}

	foo(span0);

	log_info("Create span from begin pointer and size");

	auto span1 = std::span {arr.begin(), 3};

	for (const auto &v: span1) {
		log_info("v: %i", v);
	}

	foo(span1);

	log_info("Create span from specific i-th element and size - i");

	auto span2 = std::span {arr.begin() + 2, arr.size() - 2};

	for (const auto &v: span2) {
		log_info("v: %i", v);
	}

	foo(span2);

	log_info("Test span features");

	auto value = span0[3];
	log_info("span0[3] = %i", value);

	auto front = span0.front();
	log_info("span0.front() = %i", front);

	auto back = span0.back();
	log_info("span0.back() = %i", back);

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}

	return 1;
}
