// Leka - LekaOS
// Copyright 2021-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	auto charge_input  = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
	auto corebattery   = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};
	auto mainboard_led = mbed::DigitalOut {LED1};

	log_info("Hello, World!\n\n");

	mainboard_led.write(corebattery.isCharging());

	auto on_charge_on = [&mainboard_led]() { mainboard_led.write(1); };
	corebattery.onChargeDidStart(on_charge_on);

	auto on_charge_off = [&mainboard_led]() { mainboard_led.write(0); };
	corebattery.onChargeDidStop(on_charge_off);

	while (true) {
		if (corebattery.isCharging()) {
			log_info("Battery at %.2fV (%d%%) and in charge.", corebattery.voltage(), corebattery.level());
		} else {
			log_info("Battery at %.2fV (%d%%).", corebattery.voltage(), corebattery.level());
		}
		rtos::ThisThread::sleep_for(1s);
	}
}
