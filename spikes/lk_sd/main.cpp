// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "FileManager.h"
#include "HelloWorld.h"

using namespace leka;

HelloWorld hello;
FileManager leka_sd;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Test of SD!\n\n");

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	FIL file;
	const char filename[] = "assets/images/emotion-happy.jpg";

	if (f_open(&file, filename, FA_READ) == FR_OK) {
		// Files can be opened from anywhere as long as leka_sd is called.
		printf("File %s openened. File size : %lu bytes\n", filename, f_size(&file));

		f_close(&file);
	}

	while (true) {
		auto t = Kernel::Clock::now() - start;

		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
