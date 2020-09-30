/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

const uint32_t TIMEOUT_MS = 5000;
// InterruptIn button(BUTTON1);
volatile int countdown = 9;

void trigger()
{
	Watchdog::get_instance().kick();
	countdown = 9;
}

int main()
{
	printf("\r\nTarget started.\r\n");

	Watchdog &watchdog = Watchdog::get_instance();
	watchdog.start(TIMEOUT_MS);
	// button.rise(&trigger);

	uint32_t watchdog_timeout = watchdog.get_timeout();
	printf("Watchdog initialized to %lu ms.\r\n", watchdog_timeout);
	printf(
		"Press BUTTON1 at least once every %lu ms to kick the "
		"watchdog and prevent system reset.\r\n",
		watchdog_timeout);

	while (1) {
		printf("\r%3i", countdown--);
		fflush(stdout);
		ThisThread::sleep_for(TIMEOUT_MS / 10);
	}
}
