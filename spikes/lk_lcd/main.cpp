// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "FileManager.h"
#include "HelloWorld.h"
#include "LKAVKit.h"
#include "LKCoreAV.h"
#include "lcd_properties.h"

using namespace leka;

HelloWorld hello;
LKCoreAV lk_core_av(SCREEN_BACKLIGHT_PWM);
LKAVKit lk_av_kit(lk_core_av);
FileManager sd_card;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread screen_thread;

void screenExample()
{
	printf("Screen example\n\n");

	char filename[] = "assets/images/emotion-happy.jpg";
	FIL JPEG_File;

	lk_core_av.setBrightness(0.3f);

	while (true) {
		lk_av_kit.screenSaver(30s);
		if (f_open(&JPEG_File, filename, FA_READ) == FR_OK) {
			lk_av_kit.displayImage(&JPEG_File);
			f_close(&JPEG_File);
		}
		rtos::ThisThread::sleep_for(10s);
	}
}

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	rtos::ThisThread::sleep_for(2s);

	// screen_thread.start({&leka_screen, &Screen::start});
	screen_thread.start(screenExample);
	hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
