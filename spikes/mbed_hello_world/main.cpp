// mbed Microcontroller Library
// Copyright (c) 2019 ARM Limited
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

using namespace std::chrono;

auto main() -> int
{
	while (true) {
		printf("Hello world from Mbed CE!\n");
		ThisThread::sleep_for(1s);
	}
}
