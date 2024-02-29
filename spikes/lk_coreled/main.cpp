// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include <cstddef>

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
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

auto constexpr NUM_BELT_LEDS = 20;
auto constexpr NUM_EARS_LEDS = 2;

constexpr std::array<RGB, 6> colors_available = {
	RGB::pure_green, RGB::pure_red, RGB::pure_blue, RGB::yellow, RGB::cyan, RGB::magenta,
};

constexpr std::array<RGB, NUM_BELT_LEDS> color_belt_array = {
	RGB::cyan,	  RGB::magenta, RGB::yellow,  RGB::cyan,	RGB::magenta, RGB::yellow,	RGB::cyan,
	RGB::magenta, RGB::yellow,	RGB::cyan,	  RGB::magenta, RGB::yellow,  RGB::cyan,	RGB::magenta,
	RGB::yellow,  RGB::cyan,	RGB::magenta, RGB::yellow,	RGB::cyan,	  RGB::magenta,
};

constexpr std::array<RGB, NUM_EARS_LEDS> color_ears_array = {RGB::cyan, RGB::magenta};

CoreSPI corespi_belt(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
CoreSPI corespi_ears(LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK);

CoreLED<NUM_BELT_LEDS> belt(corespi_belt);
CoreLED<NUM_EARS_LEDS> ears(corespi_ears);

void changeColor(interface::LED &e, interface::LED &b)
{
	static auto index = uint8_t {0};

	if (index < colors_available.size()) {
		e.setColor(colors_available.at(index));
		e.show();

		b.setColor(colors_available.at(index));
		b.show();

		index++;
	} else if (index == colors_available.size()) {
		e.setColorWithArray(color_ears_array);
		e.show();

		b.setColorWithArray(color_belt_array);
		b.show();

		index++;
	} else {
		static auto led = uint8_t {0};
		e.setColorAtIndex(led / 10, RGB::pure_green);
		e.show();

		b.setColorAtIndex(led, RGB::pure_green);
		b.show();

		led++;
		if (led == NUM_BELT_LEDS) {
			led = 0;
		}
	}
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	HelloWorld hello;
	hello.start();
	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	{
		changeColor(ears, belt);
		rtos::ThisThread::sleep_for(1s);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
