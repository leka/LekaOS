// Leka - LekaOS
// Copyright 2020 Embedded Planet
// Copyright 2020 ARM Limited
// SPDX-License-Identifier: Apache-2.0
// Original file: https://github.com/mcu-tools/mcuboot/blob/main/boot/mbed/mcuboot_main.cpp

#include "mbed_application.h"
#include "mbed_stats.h"
#include "mbedtls/platform.h"

#include "drivers/InterruptIn.h"
#include "drivers/Watchdog.h"
#include "rtos/ThisThread.h"

#include "ConfigKit.h"
#include "CoreBattery.h"
#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreLED.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreQSPI.h"
#include "CoreSPI.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "FirmwareKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"

using namespace std::chrono;
using namespace leka;

//
// MARK: - Global definitions
//

namespace {

namespace bootloader {

	constexpr auto version = uint8_t {2};

}

namespace os {

	constexpr auto start_address = uint32_t {0x8040000 + 0x1000};	// Start application address + header

	auto version_path = "/fs/sys/os-version";

}	// namespace os

namespace battery {

	namespace charge {

		auto status_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};

	}

	constexpr auto default_hysteresis_offset = uint8_t {5};
	constexpr auto minimum_working_level	 = float {8.0};

	auto cells = CoreBattery {PinName::BATTERY_VOLTAGE, battery::charge::status_input};

}	// namespace battery

namespace sd {

	namespace internal {

		auto bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
		auto fs = FATFileSystem {"fs"};

		constexpr auto default_frequency = uint64_t {25'000'000};

	}	// namespace internal

	void init()
	{
		internal::bd.init();
		internal::bd.frequency(internal::default_frequency);
		internal::fs.mount(&internal::bd);
	}

}	// namespace sd

namespace factory_reset {

	constexpr auto default_limit = uint8_t {10};

	namespace internal {

		constexpr auto factory_reset_counter_path = "/fs/usr/share/factory_reset_counter";

		auto qspi	 = CoreQSPI();
		auto manager = CoreFlashManagerIS25LP016D(qspi);
		auto flash	 = CoreFlashIS25LP016D(qspi, manager);

	}	// namespace internal

	auto firmwarekit = FirmwareKit(internal::flash, FirmwareKit::DEFAULT_CONFIG);

	void initializeExternalFlash()
	{
		internal::flash.reset();
		internal::qspi.setDataTransmissionFormat();
		internal::qspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
	}

	auto getCounter() -> uint8_t
	{
		if (FileManagerKit::file_is_missing(internal::factory_reset_counter_path)) {
			return default_limit + 1;
		}

		auto file = FileManagerKit::File {internal::factory_reset_counter_path, "r"};

		auto data = std::array<uint8_t, 1> {};
		file.read(data);

		return data.front();
	}

	void setCounter(uint8_t value)
	{
		FileManagerKit::File file {internal::factory_reset_counter_path, "w+"};

		if (!file.is_open()) {
			return;
		}

		auto output = std::to_array<uint8_t>({value});
		file.write(output);
	}

	void incrementCounter()
	{
		auto counter = factory_reset::getCounter();
		counter += 1;
		setCounter(counter);
	}

	void resetCounter()
	{
		setCounter(0);
	}

	void applyFactoryReset()
	{
		firmwarekit.loadFactoryFirmware();
		boot_set_pending(1);
	}

}	// namespace factory_reset

namespace config {

	auto bootloader_version = Config {"/fs/sys/bootloader-version", {bootloader::version}};
	auto battery_hysteresis_offset =
		Config {"/fs/etc/bootloader-battery_hysteresis", {battery::default_hysteresis_offset}};
	auto factory_reset_limit = Config {"/fs/etc/bootloader-reboots_limit", {factory_reset::default_limit}};

	auto configkit = ConfigKit {};

	void setBootloaderVersion()
	{
		auto ret = configkit.write(bootloader_version, bootloader_version.default_value());

		if (!ret) {
			log_error("Error writing config bootloader version");
		}
	}

	void setOSVersion(uint8_t major, uint8_t minor, uint16_t revision)
	{
		FileManagerKit::File file {os::version_path, "w+"};

		if (!file.is_open()) {
			return;
		}

		auto output = std::array<char, 14> {};
		snprintf(output.data(), std::size(output), "%i.%i.%i", major, minor, revision);
		file.write(output);
	}

	auto batteryHysteresisOffset() -> uint8_t
	{
		return configkit.read(config::battery_hysteresis_offset);
	}

	auto shouldApplyFactoryReset() -> bool
	{
		auto counter = factory_reset::getCounter();
		auto limit	 = configkit.read(config::factory_reset_limit);

		return counter > limit;
	}

}	// namespace config

namespace leds {

	namespace internal {

		namespace ears {

			auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
			constexpr auto size = 2;

		}	// namespace ears

		namespace belt {

			auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
			constexpr auto size = 20;

		}	// namespace belt

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	namespace internal {

		constexpr auto light_blue = RGB {0x00, 0x22, 0x43};

		void blink()
		{
			leds::ears.setColor(light_blue);
			leds::ears.show();
			rtos::ThisThread::sleep_for(100ms);
			leds::ears.setColor(RGB::black);
			leds::ears.show();
		}

	}	// namespace internal

	namespace blink {

		void lowEnergy()
		{
			internal::blink();
		}

		void mediumEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
		}

		void highEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
		}

	}	// namespace blink

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds

namespace motors {

	namespace left {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace left

	namespace right {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_RIGHT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace right

	void turnOff()
	{
		left::motor.stop();
		right::motor.stop();
	}

}	// namespace motors

}	// namespace

namespace leka::logger {

namespace stats {

	auto cpu   = mbed_stats_cpu_t {};
	auto stack = mbed_stats_stack_t {};
	auto heap  = mbed_stats_heap_t {};

	auto kick_count = uint32_t {0};

	auto start = rtos::Kernel::Clock::now();
	auto stop  = rtos::Kernel::Clock::now();
	auto delta = static_cast<int>((stop - start).count());

	auto ble_connected	 = uint8_t {};
	auto battery_level	 = uint8_t {};
	auto charging_status = uint8_t {};

	auto sleep_ratio	  = uint8_t {};
	auto deep_sleep_ratio = uint8_t {};

	auto stack_used_delta	 = int32_t {};
	auto stack_used_size	 = uint32_t {};
	auto stack_reserved_size = uint32_t {};
	auto stack_used_ratio	 = uint8_t {};

	auto heap_used_delta	= int32_t {};
	auto heap_used_size		= uint32_t {};
	auto heap_reserved_size = uint32_t {};
	auto heap_used_ratio	= uint8_t {};

}	// namespace stats

void log_stats()
{
	++stats::kick_count;

	stats::stop	 = rtos::Kernel::Clock::now();
	stats::delta = static_cast<int>((stats::stop - stats::start).count());
	stats::start = stats::stop;

	stats::battery_level   = battery::cells.level();
	stats::charging_status = battery::cells.isCharging() ? 1 : 0;

	mbed_stats_cpu_get(&stats::cpu);

	stats::sleep_ratio = static_cast<uint8_t>(((stats::cpu.sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
	stats::deep_sleep_ratio =
		static_cast<uint8_t>(((stats::cpu.deep_sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));

	mbed_stats_stack_get(&stats::stack);

	stats::stack_used_delta	   = static_cast<int32_t>(stats::stack.max_size - stats::stack_used_size);
	stats::stack_used_size	   = stats::stack.max_size;
	stats::stack_reserved_size = stats::stack.reserved_size;
	stats::stack_used_ratio	   = static_cast<uint8_t>((stats::stack_used_size * 100) / stats::stack_reserved_size);

	mbed_stats_heap_get(&stats::heap);

	stats::heap_used_delta	  = static_cast<int32_t>(stats::heap.current_size - stats::heap_used_size);
	stats::heap_used_size	  = stats::heap.current_size;
	stats::heap_reserved_size = stats::heap.reserved_size;
	stats::heap_used_ratio	  = static_cast<uint8_t>((stats::heap_used_size * 100) / stats::heap_reserved_size);

	log_info(
		"dt: %i, kck: %u, ble: %u, lvl: %u%%, chr: %u, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
		"%u], hur: %u%% (%+i)[%u/%u]",
		stats::delta, stats::kick_count, stats::ble_connected, stats::battery_level, stats::charging_status,
		stats::sleep_ratio, stats::deep_sleep_ratio, stats::stack_used_ratio, stats::stack_used_delta,
		stats::stack_used_size, stats::stack_reserved_size, stats::heap_used_ratio, stats::heap_used_delta,
		stats::heap_used_size, stats::heap_reserved_size);
}

}	// namespace leka::logger

//
// MARK: - main()
//

auto main() -> int
{
	leds::turnOff();
	motors::turnOff();

	logger::init();

	rtos::ThisThread::sleep_for(50ms);

	log_info("\n\n");
	log_info("Hello, Bootloader v%i!", bootloader::version);
	log_info("Arm GNU Toolchain v%s\n\n", __VERSION__);

	rtos::ThisThread::sleep_for(50ms);

	sd::init();

	// ? As bootloader, os & sd card can evelove independently, there is no way to know which
	// ? version of the bootloader is being used a priori.
	// ? On first run, we write the bootloader version to a special config file, to allow us to:
	// ?    - share the information with the os
	// ?    - guarantee the persistence of the information in case of sd card is changed
	// ?    - or in case of os update/modification
	config::setBootloaderVersion();

	while (battery::cells.level() < 0 + config::batteryHysteresisOffset()) {
		if (battery::cells.isCharging() && battery::cells.voltage() < battery::minimum_working_level) {
			leds::blink::lowEnergy();
		} else if (battery::cells.isCharging() && battery::cells.voltage() < leka::CoreBattery::Capacity::empty) {
			leds::blink::mediumEnergy();
		} else if (battery::cells.isCharging()) {
			leds::blink::highEnergy();
		}

		logger::log_stats();
		rtos::ThisThread::sleep_for(4s);
	}

	if (config::shouldApplyFactoryReset()) {
		factory_reset::initializeExternalFlash();

		factory_reset::applyFactoryReset();
		factory_reset::resetCounter();

	} else {
		factory_reset::incrementCounter();
	}

	auto start_address = os::start_address;

	if (battery::cells.isCharging() && battery::cells.voltage() > CoreBattery::Capacity::quarter) {
		// Initialize mbedtls crypto for use by MCUboot
		mbedtls_platform_context unused_ctx;
		if (auto ret = mbedtls_platform_setup(&unused_ctx); ret != 0) {
			exit(ret);
		}

		// Create handler to get application information
		auto boot_handler = boot_rsp {};
		if (auto ret = boot_go(&boot_handler); ret != FIH_SUCCESS) {
			exit(ret);
		}

		start_address = boot_handler.br_image_off + boot_handler.br_hdr->ih_hdr_size;

		auto os_version = boot_handler.br_hdr->ih_ver;
		config::setOSVersion(os_version.iv_major, os_version.iv_minor, os_version.iv_revision);
	}

	// Run the application in the primary slot
	// Add header size offset to calculate the actual start address of application
	mbed_start_application(start_address);
}
