// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "LekaBluetooth.h"

static BufferedSerial serial(USBTX, USBRX, 9600);

Bluetooth leka_bluetooth;

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};
uint16_t text_length;

Thread thread_bluetooth;

#include "FlexUtils.h"
Thread thread_flex;

int main(void)
{
	printf("\nHello, Flex Test!\n\n");

	while (!leka_bluetooth.isPaired()) {
		printf("Bluetooth is trying to pair...\n");
		leka_bluetooth.pairing();
		rtos::ThisThread::sleep_for(1s);
	}
	text_length = sprintf(buff, "BM64 paired\n");
	leka_bluetooth.sendMessage(buff, text_length);

	rtos::ThisThread::sleep_for(500ms);
	thread_flex.start(flex_thread);

	while (true) {
		leka_bluetooth.checkResponse(true);
		rtos::ThisThread::sleep_for(20ms);
	}

	return 0;
}