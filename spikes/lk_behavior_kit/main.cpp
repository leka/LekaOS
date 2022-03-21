// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "BehaviorKit.h"
#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSPI.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

namespace animations {

	auto thread		 = rtos::Thread {};
	auto event_flags = CoreEventFlags {};

}	// namespace animations

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {spi::ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {spi::belt};

}	// namespace leds

auto ledkit = LedKit {leds::animations::thread, leds::animations::event_flags, leds::ears, leds::belt};

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

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto videokit = VideoKit {};

auto behaviorkit = BehaviorKit {videokit, ledkit, motor::left, motor::right};

HelloWorld hello;

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

	log_info("Hello, World!\n\n");

	initializeSD();
	ledkit.init();
	videokit.initializeScreen();

	hello.start();

	while (true) {
		behaviorkit.launching();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.sleeping();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.blinkGreen();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.spinBlink();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.fire();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.sprinkles();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.rainbow();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);
	}
}
