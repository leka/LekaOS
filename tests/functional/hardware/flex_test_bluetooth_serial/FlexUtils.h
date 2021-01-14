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
	if (leka_rfid.checkConnected() == false) {
		text_length = sprintf(buff, "RFID is not connected\n");
		leka_bluetooth.sendMessage(buff, text_length);

		return false;
	}

	/* Initialized RFID */
	while (!leka_rfid.setIEC14443()) {
		rtos::ThisThread::sleep_for(1s);
	}
	while (!leka_rfid.setReceiverGain()) {
		rtos::ThisThread::sleep_for(1s);
	}
	text_length = sprintf(buff, "RFID enable\n");
	leka_bluetooth.sendMessage(buff, text_length);

	/* Initialized LEDs */
	initLed();

	rtos::ThisThread::sleep_for(1s);
	for (uint8_t index = 0; index < 20; index++) {
		leds[index] = CRGB::Black;
	}
	FastLED.show();

	return true;
}

void readRFID()
{
	text_length = sprintf(buff, "\n\nRFID - Ask reader");
	leka_bluetooth.sendMessage(buff, text_length);

	text_length = sprintf(buff, "\nAnswer type 1:");
	leka_bluetooth.sendMessage(buff, text_length);

	leka_rfid.sendReceive(2);
	text_length = leka_rfid.getAnswer(buff);
	leka_bluetooth.sendMessage(buff, text_length);

	text_length = sprintf(buff, "\nAnswer type 2:");
	leka_bluetooth.sendMessage(buff, text_length);

	leka_rfid.sendReceive(3);
	text_length = leka_rfid.getAnswer(buff);
	leka_bluetooth.sendMessage(buff, text_length);

	return;
}

void showLEDs(uint8_t R, uint8_t G, uint8_t B, uint8_t first_led, uint8_t last_led)
{
	text_length = sprintf(buff, "\n\nLEDs show");
	leka_bluetooth.sendMessage(buff, text_length);

	CRGB color = {R, G, B};

	first_led = (first_led >= NUM_LEDS) ? NUM_LEDS - 1 : first_led;
	last_led  = (last_led < first_led) ? first_led : last_led;
	last_led  = (last_led >= NUM_LEDS) ? NUM_LEDS - 1 : last_led;

	for (uint8_t index = first_led; index < last_led + 1; index++) {
		leds[index] = color;
	}
	FastLED.show();

	return;
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
			case 0x4C:							// For L(EDs)
				if (instruction[1] == 0x42) {	// For B(elt)
					showLEDs(instruction[2], instruction[3], instruction[4], instruction[5], instruction[6]);
				} else if (instruction[1] == 0x45) {   // For E(ars)
					continue;						   // Not implemented
				}
				break;
			case 0x52:	 // For R(FID)
				readRFID();
				break;
			default:
				break;
		}
	}
}

#endif	 // _LEKA_OS_SPIKE_FLEX_UTILS_H_