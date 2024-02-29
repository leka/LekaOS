// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>
#include <deque>

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "CoreMicrophone.h"
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

// SMA - Small Moving Average
template <uint8_t N, class input_t = uint16_t, class sum_t = uint32_t>
class SMA
{
  public:
	SMA() = default;

	auto operator()(input_t input) -> input_t
	{
		sum -= previousInputs[index];
		sum += input;
		previousInputs[index] = input;
		if (++index == N) {
			index = 0;
		}
		return (sum + (N / 2)) / N;
	}

	static_assert(sum_t(0) < sum_t(-1),
				  "Error: sum data type should be an unsigned integer, otherwise, "
				  "the rounding operation in the return statement is invalid.");

  private:
	sum_t sum	  = 0;
	uint8_t index = 0;

	std::array<input_t, N> previousInputs {};
};

// RMS - Root Mean Square
auto RMS(std::deque<int> &data, int newvalue) -> int
{
	int square = 0;
	float mean = 0;
	float root = 0;

	data.push_back(newvalue);
	for (auto value: data) {
		square += value * value;
	}
	data.pop_front();

	mean = static_cast<float>(square) / static_cast<float>(data.size());

	root = std::sqrt(mean);

	return static_cast<int>(root);
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	// RMS vs SMA value (un/comment to test)
	// static auto sma_filter = SMA<5> {};
	auto rms_buffer = std::deque<int> {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	CoreMicrophone microphone(MCU_MIC_INPUT);

	{
		auto rawValue = static_cast<int>(1000 * microphone.readVolume());

		if (rawValue <= 458) {
			rawValue = std::abs(rawValue - 458) + 458;
		}

		// RMS vs SMA value (un/comment to test)
		// auto output = sma_filter(rawValue);
		auto output = RMS(rms_buffer, rawValue);

		// TODO (@john_doe): print floats
		if (rawValue > 650) {
			log_info("Peak: %d | Mean: %d", rawValue, output);
			rtos::ThisThread::sleep_for(6ms);
		}
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
