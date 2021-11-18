// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "events/mbed_events.h"
#include "rtos/ThisThread.h"

#include "BLEGap.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;
using namespace leka;

static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

auto main() -> int
{
	logger::init();

	log_info("Hello, World!");

	auto &ble	 = BLE::Instance();
	auto ble_gap = BLEGap {ble, event_queue};
	ble_gap.setDeviceName("Leka_BLEGap");

	ble_gap.start();

	event_queue.dispatch_forever();

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
