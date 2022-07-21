// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

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

		auto event_loop = EventLoopKit {};

	}	// namespace animations

}	// namespace internal

auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

void turnOn()
{
	ears.setColor(RGB::pure_blue);
	belt.setColor(RGB::pure_blue);
	ears.show();
	belt.show();
}
void turnOff()
{
	ears.setColor(RGB::black);
	belt.setColor(RGB::black);
	ears.show();
	belt.show();
}

auto ledkit = LedKit {leds::internal::animations::event_loop, leds::ears, leds::belt};

}	// namespace leds
