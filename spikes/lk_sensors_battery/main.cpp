// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalIn.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	auto charge_input = mbed::DigitalIn {PinName::BATTERY_CHARGE_STATUS};
	auto corebattery  = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	while (true) {
		if (corebattery.isCharging()) {
			log_info("Battery at %.2fV and in charge.", corebattery.getVoltage());
		} else {
			log_info("Battery at %.2fV.", corebattery.getVoltage());
		}
		rtos::ThisThread::sleep_for(1s);
	}
}
