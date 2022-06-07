// Leka - LekaOS
// Copyright (c) 2022 Arm Limited and affiliates.
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"
#include "drivers/Ticker.h"
#include "drivers/Watchdog.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_stats.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBufferedSerial.h"
#include "CoreRFIDReader.h"
#include "CriticalSection.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "RFIDKit.h"

using namespace leka;
using namespace std::chrono;

namespace {

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 12000ms;
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

			static auto ble_connected	= uint8_t {};
			static auto battery_level	= uint8_t {};
			static auto charging_status = uint8_t {};

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
					"dt: %i, kck: %u, ble: %u, lvl: %u%%, chr: %u, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, ble_connected, battery_level, charging_status, sleep_ratio, deep_sleep_ratio,
					stack_used_ratio, stack_used_delta, stack_used_size, stack_reserved_size, heap_used_ratio,
					heap_used_delta, heap_used_size, heap_reserved_size);

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

namespace rfid {

	auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto reader = CoreRFIDReader(serial);

}	// namespace rfid

namespace robot {

	void handleCard(const MagicCard &card)
	{
		// ? Test 1 - Watchdog reset on critical section locked (ISR disabled) --> ✅
		static auto is_critical_section_locked = false;
		if (card == MagicCard::number_1) {
			log_debug("Critical section lock/unlock");
			rtos::ThisThread::sleep_for(200ms);
			if (is_critical_section_locked) {
				CriticalSection::unlock();
				is_critical_section_locked = false;
				log_debug("Unlocking critical section");   // ? Never reached
				return;
			}
			log_debug("Locking critical section");
			rtos::ThisThread::sleep_for(200ms);
			CriticalSection::lock();
			is_critical_section_locked = true;
		}

		// ? Test 2 - Watchdog reset on infinite loop w/o sleep --> ✅
		if (card == MagicCard::number_2) {
			log_debug("Infinite loop w/o sleep");
			rtos::ThisThread::sleep_for(200ms);
			while (true) {
				log_debug("To infinity and beyond!...");
			}
		}

		// ? Reset robot
		static auto emergency_stop_iteration = 0;
		if (card == MagicCard::emergency_stop) {
			++emergency_stop_iteration;
			log_debug("Emergency stop called calls: %i", emergency_stop_iteration);
			if (emergency_stop_iteration == 7) {
				system_reset();
			}
		}
	}

}	// namespace robot

auto rfidkit = RFIDKit(rfid::reader);
auto hello	 = HelloWorld();

}	// namespace

auto main() -> int
{
	logger::init();

	// ! ⚠️ Sleep is needed for components (e.g. RFID) to initialize
	rtos::ThisThread::sleep_for(2s);

	watchdog::start();
	hello.start();

	rfidkit.init();
	rfidkit.onTagActivated([](const MagicCard &card) { robot::handleCard(card); });

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n\tHello, spike Watchdog vs ISR!\n");

	rtos::ThisThread::sleep_for(1s);

	while (true) {
		// ? Un/Comment one or the other for testing
		log_info("Main thread still alive");
		// printf("Main thread still alive\n");
		rtos::ThisThread::sleep_for(1s);
	}
}
