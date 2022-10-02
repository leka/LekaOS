// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "mbed.h"

#include "LogKit.h"

void watchdog_thread()
{
	while (true) {
		Watchdog::get_instance().kick();
		log_info("Watchdog has been kicked");
		rtos::ThisThread::sleep_for(4000ms);
	}
}
