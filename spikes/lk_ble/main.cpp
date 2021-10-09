// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LKBLE.h"

using namespace std::chrono;

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
	event_queue.call(mbed::Callback<void()>(&context->ble, &BLE::processEvents));
}

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_sink_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	BLE &ble = BLE::Instance();
	ble.onEventsToProcess(schedule_ble_events);

	HeartrateDemo demo(ble, event_queue);
	demo.start();

	rtos::Thread thread_ble_event_queue;
	thread_ble_event_queue.start(callback(&event_queue, &EventQueue::dispatch_forever));

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
