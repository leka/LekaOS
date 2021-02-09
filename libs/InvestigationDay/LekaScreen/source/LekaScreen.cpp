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

	lk_core_av.Init();
	leka::draw::toolsInit();

	while (true) {
		leka::screensaver::run(43200s);	  // 12 hours
	}

	printf("End of Screen example\n\n");
}
