// Leka - LekaOS
// Copyright 2021-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"
#include "rtos/ThisThread.h"

#include "BatteryKit.h"
#include "CoreBattery.h"
#include "CoreTicker.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto charge_input  = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
auto corebattery   = CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};
auto batterykit	   = BatteryKit {corebattery};
auto mainboard_led = mbed::DigitalOut {LED1};

void logBatteryNewLevel(uint8_t battery_new_level)
{
	if (corebattery.isCharging()) {
		log_info("Battery at %d%%, voltage at %f and in charge.", battery_new_level, corebattery.voltage());
	} else {
		log_info("Battery at %d%%, voltage at %f.", battery_new_level, corebattery.voltage());
	}
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	mainboard_led.write(corebattery.isCharging());

	auto on_charge_on = []() { mainboard_led.write(1); };
	corebattery.onChargeDidStart(on_charge_on);

	auto on_charge_off = []() { mainboard_led.write(0); };
	corebattery.onChargeDidStop(on_charge_off);

	batterykit.onDataUpdated(logBatteryNewLevel);
	batterykit.onLowBattery([] { mainboard_led = !mainboard_led; });

	batterykit.startEventHandler();

	while (true) {
		rtos::ThisThread::sleep_for(1s);
	}
}
