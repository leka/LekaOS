// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

//
// MARK: - Global definitions
//

namespace {

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

	namespace animations {

		auto thread		 = rtos::Thread {};
		auto event_flags = CoreEventFlags {};

	}	// namespace animations

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

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

	void spinLeft(float speed)
	{
		left::motor.spin(Rotation::clockwise, speed);
		right::motor.spin(Rotation::clockwise, speed);
	}

	void spinRight(float speed)
	{
		left::motor.spin(Rotation::counterClockwise, speed);
		right::motor.spin(Rotation::counterClockwise, speed);
	}

	void turnOff()
	{
		left::motor.stop();
		right::motor.stop();
	}

}	// namespace motors

auto hello	= HelloWorld {};
auto ledkit = LedKit {leds::animations::thread, leds::animations::event_flags, leds::ears, leds::belt};

}	// namespace

auto main() -> int
{
	logger::init();
	leds::turnOff();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		auto reinforcer_duration = 3000ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 3200ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 3400ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 3600ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 3800ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 4000ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 4200ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 4400ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();

		rtos::ThisThread::sleep_for(3s);

		reinforcer_duration = 4600ms;
		log_info("Durée : %ims", reinforcer_duration);
		motors::spinLeft(0.5);
		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(reinforcer_duration);
		ledkit.stop();
		motors::turnOff();
	}
}
