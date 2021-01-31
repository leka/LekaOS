// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaScreen.h"

using namespace mbed;
using namespace std::chrono;

leka::LKScreen _screen(otm8009a_model);
leka::Display display(_screen);

Screen::Screen() {}

void Screen::clear(uint32_t ColorIndex)
{
	leka::drawClear(ColorIndex);
}

void Screen::showFace()
{
	char filename[] = "assets/images/emotion-happy.jpg";
	FIL JPEG_File;

	if (f_open(&JPEG_File, filename, FA_READ) == FR_OK) {
		leka::displayImage(&JPEG_File);
		f_close(&JPEG_File);
	} else
		printf("Failed to open file %s \n\r", filename);
}

void Screen::start()
{
	printf("Screen example\n\n");

	display.Init();
	leka::drawingToolsInit(otm8009a_model);

	showFace();
	// rtos::ThisThread::sleep_for(30s);
	/* TO DELETE */
	for (int i = 0; i < 21; i++) {
		if (i % 2) {
			display.turnOff();
			display.rotateUpsideDown(true);
		} else {
			display.turnOn();
			display.rotateUpsideDown(false);
		}
		display.setBrightness((float)(i / 21.0f));
		rtos::ThisThread::sleep_for(1s);
	}
	display.setBrightness(0.3f);
	/* END OF TO DELETE */

	while (true) {
		leka::screenSaver();
		rtos::ThisThread::sleep_for(1ms);
	}

	printf("End of Screen example\n\n");
}
