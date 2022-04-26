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

auto hello	= HelloWorld {};
auto ledkit = LedKit {leds::animations::thread, leds::animations::event_flags, leds::ears, leds::belt};

}	// namespace

auto main() -> int
{
	logger::init();
	//	leds::turnOff();

	log_info("Hello, World!\n\n");

	ledkit.init();

	hello.start();
	// ne pas oublier le set avant start stop run
	while (true) {
		log_info("animation::heartbeating");
		ledkit.start(&LedKit::animation::heartbeat);
		log_info("animation a été lancée");
		rtos::ThisThread::sleep_for(1s);
	}
}
