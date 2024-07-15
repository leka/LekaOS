// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceFileExchange.h"
#include "BLEServiceMonitoring.h"
#include "BLEServiceUpdate.h"

#include "CircularQueue.h"
#include "CoreEventQueue.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "FileReception.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

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

auto level			 = uint8_t {0};
auto charging_status = bool {false};

auto service_device_information = BLEServiceDeviceInformation {};
auto service_battery			= BLEServiceBattery {};
auto service_monitoring			= BLEServiceMonitoring {};
auto service_file_exchange		= BLEServiceFileExchange {};
auto service_update				= BLEServiceUpdate {};

auto services = std::to_array<interface::BLEService *>(
	{&service_device_information, &service_battery, &service_monitoring, &service_update, &service_file_exchange});

auto blekit = BLEKit {};

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
auto file_reception_handler = FileReception {};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n\n");

	blekit.setServices(services);
	std::array<uint8_t, 33> serial_number = {"LK-2202-003300294E5350092038384B"};
	service_device_information.setSerialNumber(serial_number);

	auto os_version = Version {123, 234, 45678};
	service_device_information.setOSVersion(os_version);

	blekit.onConnectionCallback([] { log_info("Connected !"); });
	blekit.onDisconnectionCallback([] { log_info("Disconnected !"); });
	blekit.onMTUNegotiated([](uint16_t new_mtu) { service_monitoring.setNegotiatedMtu(new_mtu); });

	blekit.init();

	initializeSD();

	service_file_exchange.onSetFileExchangeState(
		[](bool new_state) { log_info("New FileExchange state is: %s", (new_state ? "on" : "off")); });

	service_file_exchange.onFilePathReceived(
		[](std::span<const char> path) { file_reception_handler.setFilePath(path.data()); });

	service_file_exchange.onClearFileRequested([] { file_reception_handler.clearFile(); });

	service_file_exchange.onFileDataReceived(
		[](std::span<const uint8_t> buffer) { file_reception_handler.onPacketReceived(buffer); });

	{
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(1s);

		log_info("Is connected: %d", blekit.isConnected());

		service_battery.setBatteryLevel(level);
		++level;

		charging_status = !charging_status;
		service_monitoring.setChargingStatus(charging_status);

		log_info("Screensaver enable: %d", service_monitoring.isScreensaverEnable());

		auto version = service_update.getVersion();
		log_info("Requested version: %d.%d.%d", version.major, version.minor, version.revision);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
