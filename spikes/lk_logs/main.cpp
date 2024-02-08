// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed_stats.h"

#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

//
// MARK: - Global definitions
//

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

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	auto cpu   = mbed_stats_cpu_t {};
	auto stack = mbed_stats_stack_t {};
	auto heap  = mbed_stats_heap_t {};

	auto logs = [&cpu, &stack, &heap] {
		++kick_count;

		stop  = rtos::Kernel::Clock::now();
		delta = static_cast<int>((stop - start).count());

		mbed_stats_cpu_get(&cpu);

		idle_ratio		 = static_cast<uint8_t>(((cpu.idle_time / 1000 * 100) / (cpu.uptime / 1000)));
		sleep_ratio		 = static_cast<uint8_t>(((cpu.sleep_time / 1000 * 100) / (cpu.uptime / 1000)));
		deep_sleep_ratio = static_cast<uint8_t>(((cpu.deep_sleep_time / 1000 * 100) / (cpu.uptime / 1000)));

		mbed_stats_stack_get(&stack);

		stack_used_delta	= static_cast<int32_t>(stack.max_size - stack_used_size);
		stack_used_size		= stack.max_size;
		stack_reserved_size = stack.reserved_size;
		stack_used_ratio	= static_cast<uint8_t>((stack_used_size * 100) / stack_reserved_size);

		mbed_stats_heap_get(&heap);

		heap_used_delta	   = static_cast<int32_t>(heap.current_size - heap_used_size);
		heap_used_size	   = heap.current_size;
		heap_reserved_size = heap.reserved_size;
		heap_used_ratio	   = static_cast<uint8_t>((heap_used_size * 100) / heap_reserved_size);

		log_info(
			"dt: %i, kck: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
			"%u], hur: %u%% (%+i)[%u/%u]",
			delta, kick_count, idle_ratio, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta,
			stack_used_size, stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size, heap_reserved_size);

		start = rtos::Kernel::Clock::now();
	};

	while (true) {
		logs();
		rtos::ThisThread::sleep_for(5s);
	}
}
