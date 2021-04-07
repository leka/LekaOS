// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <mbed.h>

#include "LKBLE.h"

Thread thread_ble_event_queue;

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

	thread_ble_event_queue.start(callback(&event_queue, &EventQueue::dispatch_forever));

	while (true) {
		rtos::ThisThread::sleep_for(5s);
	}
}
