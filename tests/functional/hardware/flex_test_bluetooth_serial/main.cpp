// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	printf("\nHello, Flex Test!\n\n");

	while (true) {
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}