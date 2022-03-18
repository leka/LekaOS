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

//
// MARK: - Global definitions
//

namespace {

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

		namespace animations {

			auto thread		 = rtos::Thread {};
			auto event_flags = CoreEventFlags {};

		}	// namespace animations

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	auto kit = LedKit {internal::animations::thread, internal::animations::event_flags, ears, belt};

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
auto hal	  = CoreSTM32Hal {};
auto videokit = VideoKit {hal};
VideoKit_DeclareIRQHandlers(videokit);

auto behaviorkit = BehaviorKit {videokit, leds::kit, motors::left::motor, motors::right::motor};
auto hello		 = HelloWorld {};

}	// namespace

auto main() -> int
{
	logger::init();

	leds::turnOff();
	motors::turnOff();

	leds::kit.init();
	sd::init();
	videokit.initializeScreen();

	log_info("Hello, World!\n\n");

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
