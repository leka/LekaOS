// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include <cstddef>

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "ColorKit.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
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

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

	void changeColors()
	{
		RGB col1 = RGB::pure_blue;
		RGB col2 = RGB::pure_red;

		ears.setColorAtIndex(0, col1);
		ears.setColorAtIndex(1, col2);

		for (auto i = 0; i < internal::belt::size; i++) {
			RGB col = ColorKit::colorGradient(col1, col2, static_cast<float>(i) / (internal::belt::size - 1));
			belt.setColorAtIndex(i, col);
		}

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

	hello.start();
	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	while (true) {
		leds::changeColors();
		rtos::ThisThread::sleep_for(1s);
		leds::turnOff();
		rtos::ThisThread::sleep_for(1s);
	}
}
