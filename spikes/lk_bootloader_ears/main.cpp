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

	void blink_ears_high(RGB color, int cpt)
	{
		for (int i(0); i < cpt; i++) {
			leds::ears.setColor(color);
			leds::ears.show();
			rtos::ThisThread::sleep_for(100ms);
			leds::ears.setColor(RGB::black);
			leds::ears.show();

			rtos::ThisThread::sleep_for(100ms);

			leds::ears.setColor(color);
			leds::ears.show();
			rtos::ThisThread::sleep_for(100ms);
			leds::ears.setColor(RGB::black);
			leds::ears.show();

			rtos::ThisThread::sleep_for(100ms);

			leds::ears.setColor(color);
			leds::ears.show();
			rtos::ThisThread::sleep_for(100ms);
			leds::ears.setColor(RGB::black);
			leds::ears.show();

			rtos::ThisThread::sleep_for(2s);
		}
	}

}	// namespace leds

auto hello	= HelloWorld {};
auto ledkit = LedKit {leds::animations::thread, leds::animations::event_flags, leds::ears, leds::belt};

}	// namespace

auto main() -> int
{
	logger::init();
	leds::turnOff();

	log_info("Hello, World!\n\n");

	ledkit.init();

	hello.start();

	while (true) {
		rtos::ThisThread::sleep_for(3s);
		leds::blink_ears_high(leka::RGB {0x00, 0x22, 0x43}, 4);
		rtos::ThisThread::sleep_for(3s);
		leds::blink_ears_high(RGB::vert_pre, 4);
		rtos::ThisThread::sleep_for(3s);
	}
}
