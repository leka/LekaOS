// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include <cstddef>

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto constexpr NUM_BELT_LEDS = 20;
auto constexpr NUM_EARS_LEDS = 2;

constexpr std::array<RGB, 6> colors_available = {
	RGB::pure_green, RGB::pure_red, RGB::pure_blue, RGB::yellow, RGB::cyan, RGB::magenta,
};

constexpr std::array<RGB, NUM_BELT_LEDS> color_belt_array = {
	RGB::cyan,	  RGB::magenta, RGB::yellow,  RGB::cyan,	RGB::magenta, RGB::yellow,	RGB::cyan,
	RGB::magenta, RGB::yellow,	RGB::cyan,	  RGB::magenta, RGB::yellow,  RGB::cyan,	RGB::magenta,
	RGB::yellow,  RGB::cyan,	RGB::magenta, RGB::yellow,	RGB::cyan,	  RGB::magenta,
};

constexpr std::array<RGB, NUM_EARS_LEDS> color_ears_array = {RGB::cyan, RGB::magenta};

CoreSPI corespi_belt(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
CoreSPI corespi_ears(LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK);

CoreLED<NUM_BELT_LEDS> belt(corespi_belt);
CoreLED<NUM_EARS_LEDS> ears(corespi_ears);

void changeColor(interface::LED &e, interface::LED &b)
{
	static auto index = uint8_t {0};

	if (index < colors_available.size()) {
		e.setColor(colors_available.at(index));
		e.show();

		b.setColor(colors_available.at(index));
		b.show();

		index++;
	} else if (index == colors_available.size()) {
		e.setColorWithArray(color_ears_array);
		e.show();

		b.setColorWithArray(color_belt_array);
		b.show();

		index++;
	} else {
		static auto led = uint8_t {0};
		e.setColorAtIndex(led / 10, RGB::pure_green);
		e.show();

		b.setColorAtIndex(led, RGB::pure_green);
		b.show();

		led++;
		if (led == NUM_BELT_LEDS) {
			led = 0;
		}
	}
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();
	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	while (true) {
		changeColor(ears, belt);
		rtos::ThisThread::sleep_for(1s);
	}
}
