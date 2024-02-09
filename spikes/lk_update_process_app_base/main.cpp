// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreQSPI.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "FirmwareKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "QSPIFBlockDevice.h"
#include "SDBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "bootutil/bootutil.h"

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

HelloWorld hello;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

auto coreqspi		   = CoreQSPI();
auto coremanageris25lp = CoreFlashManagerIS25LP016D(coreqspi);
auto coreis25lp		   = CoreFlashIS25LP016D(coreqspi, coremanageris25lp);
auto firmwarekit	   = FirmwareKit(coreis25lp, FirmwareKit::DEFAULT_CONFIG);

auto get_secondary_bd() -> mbed::BlockDevice *
{
	static auto _bd = QSPIFBlockDevice {};

	static auto sliced_bd = mbed::SlicingBlockDevice {&_bd, 0x0, MCUBOOT_SLOT_SIZE};

	return &sliced_bd;
}

void initializeSD()
{
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, Application!\n");

	// Initialization
	hello.start();

	initializeSD();

	coreis25lp.reset();
	coreqspi.setDataTransmissionFormat();
	coreqspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);

	// Load file
	auto version = Version {.major = 1, .minor = 2, .revision = 3};
	if (auto did_load = firmwarekit.loadFirmware(version); did_load) {
		log_info("New update was loaded in external flash");
	}

	// Set ready for reboot
	if (int ret = boot_set_pending(0); ret == 0) {
		log_info("> Secondary image pending, reboot to update");
	} else {
		log_error("Failed to set secondary image pending: %d", ret);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
