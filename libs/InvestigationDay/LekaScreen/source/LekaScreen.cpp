// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaScreen.h"

using namespace std::chrono;

Screen::Screen() {}

void Screen::start()
{
	printf("Screen example\n\n");

	leka::Display display(otm8009a_model);

	display.Init();
	leka::drawingToolsInit(otm8009a_model);

	while (true) {
		leka::screenSaver(43200s);	 // 12 hours
	}

	printf("End of Screen example\n\n");
}
