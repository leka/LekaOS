// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

static mbed::BufferedSerial serial(USBTX, USBRX, 115200);

int main()
{
	printf("Hello Bootloader!\n");

	return 0;
}
