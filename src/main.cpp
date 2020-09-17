// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FastLED.h"
#include "HelloWorld.h"
// #include "SK9822Controller.h"
#include "mbed.h"

SPI spi(D11, NC, D13);

#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

HelloWorld hello;

// static BufferedSerial serial(USBTX, USBRX, 9600);

// constexpr uint8_t buff_size = 128;
// char buff[buff_size] {};

int main(void)
{
	printf("\nHello, Investigation Day!\n\n");

	FastLED.addLeds<SK9822, D11, D13, RGB>(leds, spi, NUM_LEDS);
	leds[0] = CRGB::White;
	leds[1] = CRGB::Black;
	leds[2] = CRGB::White;
	leds[3] = CRGB::Black;
	leds[4] = CRGB::White;
	FastLED.show();

	rtos::ThisThread::sleep_for(2s);

	// auto start = Kernel::Clock::now();
	hello.start();

	while (true) {
		// auto t	   = Kernel::Clock::now() - start;
		// int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
		// 					 hello.world, int(t.count() / 1000));
		// serial.write(buff, length);

		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
