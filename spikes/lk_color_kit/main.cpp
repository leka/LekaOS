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

auto constexpr NUM_BELT_LEDS = 20;
auto constexpr NUM_EARS_LEDS = 2;

CoreSPI corespi_belt(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
CoreSPI corespi_ears(LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK);

CoreLED<NUM_BELT_LEDS> belt(corespi_belt);
CoreLED<NUM_EARS_LEDS> ears(corespi_ears);

void doGradient(interface::LED &e, interface::LED &b)
{
	RGB col1 = RGB::pure_blue;
	RGB col2 = RGB::pure_red;

	e.setColorAtIndex(0, col1);
	e.setColorAtIndex(1, col2);

	for (auto i = 0; i < NUM_BELT_LEDS; i++) {
		RGB col = ColorKit::colorGradient(col1, col2, static_cast<float>(i) / (NUM_BELT_LEDS - 1));
		b.setColorAtIndex(i, col);
	}
	e.show();
	b.show();
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();
	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	while (true) {
		doGradient(ears, belt);
		rtos::ThisThread::sleep_for(1s);
	}
}
