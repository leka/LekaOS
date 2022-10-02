// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
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

		auto event_loop = EventLoopKit {};

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

auto hello	= HelloWorld {};
auto ledkit = LedKit {leds::animations::event_loop, leds::ears, leds::belt};

}	// namespace

auto main() -> int
{
	logger::init();
	leds::turnOff();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		ledkit.start(&LedKit::animation::rainbow);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::fire);
		rtos::ThisThread::sleep_for(10s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::sprinkles);
		rtos::ThisThread::sleep_for(5s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::spin_blink);
		rtos::ThisThread::sleep_for(40s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);

		ledkit.start(&LedKit::animation::blink_green);
		rtos::ThisThread::sleep_for(40s);

		ledkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
