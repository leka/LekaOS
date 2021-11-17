// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_WATCHDOG_UTILS_H_
#define _LEKA_OS_WATCHDOG_UTILS_H_

#include "rtos/Thread.h"

void startWatchdog(rtos::Thread &thread_watchdog);
void watchdogLoop();

#endif	 // _LEKA_OS_WATCHDOG_UTILS_H_
