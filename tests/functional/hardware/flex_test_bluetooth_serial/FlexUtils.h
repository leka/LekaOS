// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_FLEX_UTILS_H_
#define _LEKA_OS_SPIKE_FLEX_UTILS_H_

#include "FastLED.h"
#include "LekaRfid.h"
#include "LekaTouch.h"

RFID leka_rfid;
Touch leka_touch;

char instruction[8];

bool flex_init()
{
	// Check flex is connected and initialized components on it.

	return true;
}

void flex_thread()
{
	text_length = sprintf(buff, "Start flex\n");
	leka_bluetooth.sendMessage(buff, text_length);

	while (flex_init() == false) {
		rtos::ThisThread::sleep_for(5s);
	}

	while (true) {
		rtos::ThisThread::sleep_for(1s);

		if (leka_bluetooth.checkNewMessage() == false) {
			continue;
		}
		leka_bluetooth.getMessage(instruction);

		switch (instruction[0]) {
			default:
				break;
		}
	}
}

#endif	 // _LEKA_OS_SPIKE_FLEX_UTILS_H_