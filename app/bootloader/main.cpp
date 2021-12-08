// Leka - LekaOS
// Copyright 2020 Embedded Planet
// Copyright 2020 ARM Limited
// SPDX-License-Identifier: Apache-2.0
// Original file: https://github.com/mcu-tools/mcuboot/blob/main/boot/mbed/mcuboot_main.cpp

#include "mbed_application.h"
#include "mbedtls/platform.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalIn.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "LogKit.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"

using namespace std::chrono;
using namespace leka;

static auto serial		 = mbed::BufferedSerial {CONSOLE_TX, CONSOLE_RX, 115200};
auto charge_status_input = mbed::DigitalIn {PinName::BATTERY_CHARGE_STATUS};
auto battery			 = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_status_input};

auto main() -> int
{
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Starting MCUboot");

	while (battery.getVoltage() < CoreBattery::Capacity::empty) {
		rtos::ThisThread::sleep_for(10s);
	}

	uint32_t address = 0x8041000;

	if (battery.isCharging() && battery.getVoltage() > CoreBattery::Capacity::quarter) {
		// Initialize mbedtls crypto for use by MCUboot
		mbedtls_platform_context unused_ctx;
		if (auto ret = mbedtls_platform_setup(&unused_ctx); ret != 0) {
			log_error("Failed to setup Mbed TLS, error: %d", ret);
			exit(ret);
		}

		// Create handler to get application information
		auto boot_handler = boot_rsp {};
		if (auto ret = boot_go(&boot_handler); ret != FIH_SUCCESS) {
			log_error("Failed to locate firmware image, error: %d", ret);
			exit(ret);
		}

		address = boot_handler.br_image_off + boot_handler.br_hdr->ih_hdr_size;
	}

	// Workaround: The extra \n ensures the last trace gets flushed
	// before mbed_start_application() destroys the stack and jumps
	// to the application
	log_info("Booting firmware image at 0x%x\n", address);

	// Run the application in the primary slot
	// Add header size offset to calculate the actual start address of application
	mbed_start_application(address);
}
