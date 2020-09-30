// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <mbed.h>

#include "LKBLE.h"

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
	event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

int main()
{
	BLE &ble = BLE::Instance();

	ble.onEventsToProcess(schedule_ble_events);

	HeartrateDemo demo(ble, event_queue);

	demo.start();

	while (true) {
		rtos::ThisThread::sleep_for(5s);
	}

	return 0;
}
