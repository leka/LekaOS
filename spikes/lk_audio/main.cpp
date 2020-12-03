// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "AudioFrequency.h"
#include "HelloWorld.h"

using namespace leka;

HelloWorld hello;

AnalogOut audio_output(MCU_SOUND_OUT);
DigitalOut audio_enable(SOUND_ENABLE, 1);

AudioFrequency audio_frequency(audio_output);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello LekaOS, Audio test!\n\n");

	rtos::ThisThread::sleep_for(2s);

	audio_frequency.playFrequency(440, 1);	 // 440 Hz, 10s

	// hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
