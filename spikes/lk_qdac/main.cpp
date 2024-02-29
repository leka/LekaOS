// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreQDAC.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

//
// MARK: - Global definitions
//

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

auto corei2c = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
auto dac	 = CoreQDACMCP4728 {corei2c, 0xC2};

auto data = uint16_t {};

void printInputRegisters()
{
	for (auto channels = std::array<uint8_t, 4> {mcp4728::channel::A, mcp4728::channel::B, mcp4728::channel::C,
												 mcp4728::channel::D};
		 uint8_t channel: channels) {
		log_info("================= Channel %d ================= ", channel);
		data = dac.read(channel);
		log_info("Input Registers === Data: %x", data);
		log_info("\n\n");
	}
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	rtos::ThisThread::sleep_for(1s);

	auto channel = uint8_t {};
	log_info("Read DAC (channel 0=A, 1=B, 2=C, 3=D):\n");

	log_info("Init !\n");
	dac.init();
	rtos::ThisThread::sleep_for(100ms);
	printInputRegisters();
	rtos::ThisThread::sleep_for(2s);

	channel = 0x00;

	data = 0x0ABC;
	log_info("Writing ! Channel : %d, Data : %x\n", channel, data);
	dac.write(channel, data);
	rtos::ThisThread::sleep_for(100ms);
	printInputRegisters();
	rtos::ThisThread::sleep_for(1s);

	channel = 0x01;

	data = 0x0DEF;
	log_info("Writing ! Channel : %d, Data : %x\n", channel, data);
	dac.write(channel, data);
	rtos::ThisThread::sleep_for(100ms);
	printInputRegisters();
	rtos::ThisThread::sleep_for(1s);

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
