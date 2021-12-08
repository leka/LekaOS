// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "events/mbed_events.h"
#include "rtos/ThisThread.h"

#include "BLEGap.h"

#include "CoreBattery.h"
#include "DigitalIn.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;
using namespace leka;

static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

auto charge_input = mbed::DigitalIn {PinName::BATTERY_CHARGE_STATUS};
auto battery	  = CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};

auto getBatteryLevel() -> uint8_t
{
	auto battery_level			 = 0;
	auto current_battery_voltage = battery.getVoltage();

	if (current_battery_voltage < CoreBattery::Capacity::quarter) {
		battery_level = 0;
	} else if (current_battery_voltage < CoreBattery::Capacity::half) {
		battery_level = 25;
	} else if (current_battery_voltage < CoreBattery::Capacity::three_quarter) {
		battery_level = 50;
	} else if (current_battery_voltage < CoreBattery::Capacity::full) {
		battery_level = 75;
	} else {
		battery_level = 100;
	}

	return battery_level;
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!");

	auto &ble	 = BLE::Instance();
	auto ble_gap = BLEGap {ble, event_queue};
	ble_gap.setDeviceName("Leka_BLEGap");

	ble_gap.start();

	auto update_battery_level = [&ble_gap]() { ble_gap.setBatteryLevel(getBatteryLevel()); };
	event_queue.call_every(1s, update_battery_level);

	event_queue.dispatch_forever();

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
