// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_WATCHDOG_UTILS_H_
#define _LEKA_OS_SPIKE_WATCHDOG_UTILS_H_

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

#endif	 // _LEKA_OS_SPIKE_WATCHDOG_UTILS_H_
