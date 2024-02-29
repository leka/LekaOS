// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"

#include "CoreEventQueue.h"
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

mbed::DigitalOut blinky(LED1, 0);

class LEDManager
{
  public:
	explicit LEDManager() = default;

	void init() { event_queue.dispatch_forever(); }

	void turnOnRef(mbed::DigitalOut &led)
	{
		event_queue.call([&led] { led.write(1); });
	}

	void turnOffRef(mbed::DigitalOut &led)
	{
		event_queue.call([&led] { led.write(0); });
	}

	void blinkRef(mbed::DigitalOut &led)
	{
		for (auto i = 0; i < 10; ++i) {
			// ? this-> can be omitted but is kept for clarity
			event_queue.call([&led, this] { this->turnOnRef(led); });
			rtos::ThisThread::sleep_for(100ms);
			event_queue.call([&led, this] { this->turnOffRef(led); });
			rtos::ThisThread::sleep_for(100ms);
		}
	}

	void turnOnPtr(mbed::DigitalOut *led)
	{
		event_queue.call([led] { led->write(1); });
	}

	void turnOffPtr(mbed::DigitalOut *led)
	{
		event_queue.call([led] { led->write(0); });
	}

	void blinkPtr(mbed::DigitalOut *led)
	{
		for (auto i = 0; i < 10; ++i) {
			// ? this-> can be omitted but is kept for clarity
			event_queue.call([this, led] { this->turnOnPtr(led); });
			rtos::ThisThread::sleep_for(100ms);
			event_queue.call([this, led] { this->turnOffPtr(led); });
			rtos::ThisThread::sleep_for(100ms);
		}
	}

  private:
	CoreEventQueue event_queue {};
};

auto manager = LEDManager {};

static void void_function()
{
	log_info("void_function");
}

auto int_function_with_param(int i) -> int
{
	log_info("void_function with param: %i", i);
	return 1;
}

auto int_function_with_multiple_params(int i, const std::string &std, bool b) -> int
{
	log_info("void_function with param: %i, %s, %i", i, std.c_str(), b);
	return 1;
}

CoreEventQueue equeue {};

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n\n");

	manager.init();

	{
		log_info("Turn on ref");
		manager.turnOnRef(blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn off ref");
		manager.turnOffRef(blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn blink ref");
		manager.blinkRef(blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn on pointer");
		manager.turnOnPtr(&blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn off pointer");
		manager.turnOffPtr(&blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn blink pointer");
		manager.blinkPtr(&blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Call others");
		equeue.dispatch_forever();
		equeue.call(void_function);
		equeue.call(int_function_with_param, 42);
		int a		= 42;
		auto lambda = [&a] {
			log_info("lambda with capture: %i", a);
			return true;
		};
		equeue.call(lambda);
		equeue.call(int_function_with_multiple_params, 42, std::string {"Hello, World"}, true);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
