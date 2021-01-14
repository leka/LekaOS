// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "HelloWorld.h"
#include "LekaBluetooth.h"

using namespace leka;

HelloWorld hello;
Bluetooth leka_bluetooth;

static BufferedSerial serial(USBTX, USBRX, 9600);
DigitalOut audio_enable(SOUND_ENABLE, 1);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread bluetooth_thread;

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	bluetooth_thread.start({&leka_bluetooth, &Bluetooth::start});
	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		leka_bluetooth.sendMessage(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
