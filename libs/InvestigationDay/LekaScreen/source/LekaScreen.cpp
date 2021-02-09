// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaScreen.h"

using namespace std::chrono;

Screen::Screen() {}

void Screen::start()
{
	printf("Screen example\n\n");

	leka::LKCoreAV lk_core_av(SCREEN_BACKLIGHT_PWM);
	leka::Draw draw;
	leka::Screensaver screensaver;

	lk_core_av.initialize();
	draw.initialize();
	screensaver.start();

	while (true) {
		rtos::ThisThread::sleep_for(1h);
	}

	printf("End of Screen example\n\n");
}
