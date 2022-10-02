// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "mbed.h"

void watchdog_thread()
{
	while (true) {
		Watchdog::get_instance().kick();
		printf("Watchdog has been kicked\n");
		rtos::ThisThread::sleep_for(4000ms);
	}
}
