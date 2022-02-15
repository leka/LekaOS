// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include <cstddef>

#include <cmath>

#include "PinNames.h"

#include "drivers/HighResClock.h"
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

constexpr std::array<bRGB, NUM_BELT_LEDS> leds_belt_array = {
	bRGB::cyan,	   bRGB::magenta, bRGB::yellow,	 bRGB::cyan,	bRGB::magenta, bRGB::yellow,  bRGB::cyan,
	bRGB::magenta, bRGB::yellow,  bRGB::cyan,	 bRGB::magenta, bRGB::yellow,  bRGB::cyan,	  bRGB::magenta,
	bRGB::yellow,  bRGB::cyan,	  bRGB::magenta, bRGB::yellow,	bRGB::cyan,	   bRGB::magenta,
};

constexpr std::array<bRGB, NUM_EARS_LEDS> leds_ears_array = {bRGB::cyan, bRGB::magenta};

CoreSPI corespi_belt(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
CoreSPI corespi_ears(LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK);

CoreLED<NUM_BELT_LEDS> belt(corespi_belt);
CoreLED<NUM_EARS_LEDS> ears(corespi_ears);

auto mainMillis() -> uint32_t
{
	mbed::HighResClock::lock();
	auto t = mbed::HighResClock::now();
	mbed::HighResClock::unlock();
	std::chrono::microseconds tms = t.time_since_epoch();
	return static_cast<uint32_t>(tms.count() / 1000);
}

auto sin_coreled(float millis) -> float
{
	return sin(fmod((millis / 100), (2 * 3.14159)));
}

auto clamp_coreled(float x) -> uint8_t
{
	return (x + 1) * 16;
}

void changeColor(interface::LED &e, interface::LED &b)
{
	static auto index = uint8_t {0};

	if (index < colors_available.size()) {
		e.setLeds(colors_available.at(index));
		e.showLeds();
		b.setLeds(colors_available.at(index));
		b.showLeds();

		index++;
	} else if (index == colors_available.size()) {
		e.setLedsWithArray(leds_ears_array);
		e.showLeds();
		b.setLedsWithArray(leds_belt_array);
		b.showLeds();

		index++;
	} else {
		static auto led = uint8_t {0};
		e.setLedsAtIndex(led / 10, RGB::pure_green);
		e.showLeds();
		b.setLedsAtIndex(led, RGB::pure_green);
		b.showLeds();

		led++;
		if (led == NUM_BELT_LEDS) {
			led = 0;
		}
	}
}

void changeBrightness(interface::LED &e, interface::LED &b)
{
	auto x = (clamp_coreled(sin_coreled(mainMillis())) + clamp_coreled(sin_coreled(2 * mainMillis()))) / 2;
	e.setLeds(RGB::pure_blue, 0xE0 + x);
	e.showLeds();
	b.setLeds(RGB::pure_blue, 0xE0 + x);
	b.showLeds();
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();
	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	while (true) {
		changeBrightness(ears, belt);
		rtos::ThisThread::sleep_for(10ms);
	}
}
