// Leka - LekaOS
// Copyright 2020 Embedded Planet
// Copyright 2020 ARM Limited
// SPDX-License-Identifier: Apache-2.0
// Original file: https://github.com/mcu-tools/mcuboot/blob/main/boot/mbed/mcuboot_main.cpp

#include "mbed_application.h"
#include "mbedtls/platform.h"

#include "drivers/DigitalIn.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"

using namespace std::chrono;
using namespace leka;

constexpr auto default_address = uint32_t {0x8040000 + 0x1000};	  // Start application address + header

static auto charge_status_input = mbed::DigitalIn {PinName::BATTERY_CHARGE_STATUS};
static auto battery				= leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_status_input};

auto main() -> int
{
	while (battery.getVoltage() < CoreBattery::Capacity::empty) {
		rtos::ThisThread::sleep_for(10s);
	}

	auto address = default_address;

	if (battery.isCharging() && battery.getVoltage() > CoreBattery::Capacity::quarter) {
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

		address = boot_handler.br_image_off + boot_handler.br_hdr->ih_hdr_size;
	}

	// Run the application in the primary slot
	// Add header size offset to calculate the actual start address of application
	mbed_start_application(address);
}
