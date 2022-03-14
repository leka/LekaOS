// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto constexpr NUM_EARS_LEDS = 2;

auto corespi_ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

auto ears = CoreLED<NUM_EARS_LEDS> {corespi_ears};

HelloWorld hello;

void blink(milliseconds black_time)
{
	ears.setColor(RGB::pure_red);
	ears.show();
	rtos::ThisThread::sleep_for(100ms);
	ears.setColor(RGB::black);
	ears.show();
	rtos::ThisThread::sleep_for(black_time);
}

void blinkLowEnergy()
{
	for (auto i = 0; i < 4; ++i) {
		blink(2s);
	}
}

void blinkMediumEnergy()
{
	for (auto i = 0; i < 4; ++i) {
		blink(100ms);
		blink(2s);
	}
}

void blinkHighEnergy()
{
	for (auto i = 0; i < 4; ++i) {
		blink(100ms);
		blink(100ms);
		blink(2s);
	}
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		blinkLowEnergy();

		blinkMediumEnergy();

		blinkHighEnergy();
	}
}
