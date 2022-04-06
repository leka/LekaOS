// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
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

auto hello = HelloWorld {};

}	// namespace

auto main() -> int
{
	logger::init();
	leds::turnOff();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		leds::kit.start(&LedKit::animation::rainbow);
		rtos::ThisThread::sleep_for(10s);

		leds::kit.stop();
		rtos::ThisThread::sleep_for(1s);

		leds::kit.start(&LedKit::animation::fire);
		rtos::ThisThread::sleep_for(10s);

		leds::kit.stop();
		rtos::ThisThread::sleep_for(1s);

		leds::kit.start(&LedKit::animation::sprinkles);
		rtos::ThisThread::sleep_for(5s);

		leds::kit.stop();
		rtos::ThisThread::sleep_for(1s);

		leds::kit.start(&LedKit::animation::spin_blink);
		rtos::ThisThread::sleep_for(40s);

		leds::kit.stop();
		rtos::ThisThread::sleep_for(1s);

		leds::kit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(40s);

		leds::kit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
