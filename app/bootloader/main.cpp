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

//
// MARK: - Global definitions
//

namespace {

namespace config {

	const auto file = FileManagerKit::File {"/fs/conf/bootloader.conf"};

}

namespace os {

	constexpr auto start_address = uint32_t {0x8040000 + 0x1000};	// Start application address + header

}	// namespace os

namespace battery {

	namespace charge {

		auto status_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};

	}

	constexpr auto hysteresis_offset	 = uint8_t {5};
	constexpr auto minimum_working_level = float {8.0};

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

		void blink()
		{
			leds::ears.setColor(RGB::pure_red);
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
			rtos::ThisThread::sleep_for(2s);
		}

		void mediumEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(2s);
		}

		void highEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(2s);
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

//
// MARK: - main()
//

auto main() -> int
{
	leds::turnOff();
	motors::turnOff();

	while (battery::cells.level() < 0 + battery::hysteresis_offset) {
		if (battery::cells.isCharging() && battery::cells.voltage() < battery::minimum_working_level) {
			leds::blink::lowEnergy();
		} else if (battery::cells.isCharging() && battery::cells.voltage() < leka::CoreBattery::Capacity::empty) {
			leds::blink::mediumEnergy();
		} else if (battery::cells.isCharging()) {
			leds::blink::highEnergy();
		}

		rtos::ThisThread::sleep_for(5s);
	}

	sd::init();

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
	}

	// Run the application in the primary slot
	// Add header size offset to calculate the actual start address of application
	mbed_start_application(start_address);
}
