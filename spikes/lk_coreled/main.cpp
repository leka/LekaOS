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
auto constexpr NUM_EAR_LEDS	 = 2;

constexpr std::array<RGB, 6> colors_available = {
	RGB::pure_green, RGB::pure_red, RGB::pure_blue, RGB::yellow, RGB::cyan, RGB::magenta,
};

CoreSPI corespi_belt(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
CoreSPI corespi_ears(LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK);

CoreLED belt(corespi_belt, NUM_BELT_LEDS);
CoreLED ears(corespi_ears, NUM_EAR_LEDS);

void changeColor(interface::LED &ears, interface::LED &belt)
{
	static auto index = uint8_t {0};

	if (index < colors_available.size()) {
		ears.setColor(colors_available.at(index));
		ears.showColor();

		belt.setColor(colors_available.at(index));
		belt.showColor();

		index++;
	} else {
		ears.hideColor();
		belt.hideColor();

		index = 0;
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
		changeColor(belt, ears);
		rtos::ThisThread::sleep_for(1s);
	}
}
