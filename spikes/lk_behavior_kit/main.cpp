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

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto videokit = VideoKit {};

auto corespi_belt	  = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
auto corespi_ears	  = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
auto ears			  = CoreLED<LedKit::kNumberOfLedsEars> {corespi_ears};
auto belt			  = CoreLED<LedKit::kNumberOfLedsBelt> {corespi_belt};
auto animation_thread = rtos::Thread {};
auto event_flags	  = CoreEventFlags {};

auto ledkit = LedKit {animation_thread, event_flags, ears, belt};

auto motor_left_dir_1  = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
auto motor_left_dir_2  = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
auto motor_left_speed  = CorePwm {MOTOR_LEFT_PWM};
auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
auto motor_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
auto motor_right_speed = CorePwm {MOTOR_RIGHT_PWM};

auto motor_left	 = CoreMotor {motor_left_dir_1, motor_left_dir_2, motor_left_speed};
auto motor_right = CoreMotor {motor_right_dir_1, motor_right_dir_2, motor_right_speed};

auto behaviorkit = BehaviorKit {videokit, ledkit, motor_left, motor_right};

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
