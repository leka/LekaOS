// Leka - LekaOS
// Copyright 2020 Embedded Planet
// Copyright 2020 ARM Limited
// SPDX-License-Identifier: Apache-2.0
// Original file: https://github.com/mcu-tools/mcuboot/blob/main/boot/mbed/mcuboot_main.cpp

#include "mbed_application.h"
#include "mbedtls/platform.h"

#include "drivers/InterruptIn.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "CoreLED.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSPI.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "SDBlockDevice.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"

using namespace std::chrono;
using namespace leka;

static constexpr auto default_address = uint32_t {0x8040000 + 0x1000};	 // Start application address + header

static auto charge_status_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
static auto battery				= leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_status_input};

auto battery_level_hysteresis_offset = uint8_t {5};

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

FileManagerKit::File _configuration_file {"/fs/conf/bootloader.conf"};

static constexpr auto NUM_EARS_LEDS = 2;
static constexpr auto NUM_BELT_LEDS = 20;

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

auto ears = CoreLED<NUM_EARS_LEDS> {spi::ears};
auto belt = CoreLED<NUM_BELT_LEDS> {spi::belt};

}	// namespace leds

namespace motor {

namespace internal {

	namespace left {

		auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
		auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
		auto speed = CorePwm {MOTOR_LEFT_PWM};

	}	// namespace left
	namespace right {

		auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
		auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
		auto speed = CorePwm {MOTOR_RIGHT_PWM};

	}	// namespace right
}	// namespace internal

auto left  = CoreMotor {internal::left::dir_1, internal::left::dir_2, internal::left::speed};
auto right = CoreMotor {internal::right::dir_1, internal::right::dir_2, internal::right::speed};

}	// namespace motor

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

void turnOffLeds()
{
	leds::ears.setColor(RGB::black);
	leds::belt.setColor(RGB::black);
	leds::ears.show();
	leds::belt.show();
}

void turnOffMotors()
{
	motor::left.stop();
	motor::right.stop();
}

void blink()
{
	leds::ears.setColor(RGB::pure_red);
	leds::ears.show();
	rtos::ThisThread::sleep_for(100ms);
	leds::ears.setColor(RGB::black);
	leds::ears.show();
}

void blinkLowEnergy()
{
	blink();
	rtos::ThisThread::sleep_for(2s);
}

void blinkMediumEnergy()
{
	blink();
	rtos::ThisThread::sleep_for(100ms);
	blink();
	rtos::ThisThread::sleep_for(2s);
}

void blinkHighEnergy()
{
	blink();
	rtos::ThisThread::sleep_for(100ms);
	blink();
	rtos::ThisThread::sleep_for(100ms);
	blink();
	rtos::ThisThread::sleep_for(2s);
}

auto main() -> int
{
	turnOffLeds();
	turnOffMotors();

	while (battery.level() < 0 + battery_level_hysteresis_offset) {
		if (battery.isCharging() && battery.voltage() < 8.0) {
			blinkLowEnergy();
		} else if (battery.isCharging() && battery.voltage() < CoreBattery::Capacity::empty) {
			blinkMediumEnergy();
		} else if (battery.isCharging()) {
			blinkHighEnergy();
		}

		rtos::ThisThread::sleep_for(5s);
	}

	initializeSD();

	auto address = default_address;

	if (battery.isCharging() && battery.voltage() > CoreBattery::Capacity::quarter) {
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
