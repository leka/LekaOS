// Leka - LekaOS
// Copyright 2021-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "drivers/InterruptIn.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	auto charge_input  = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
	auto corebattery   = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	while (true) {
		if (corebattery.isCharging()) {
			log_info("Battery at %.2fV (%d%%) and in charge.", corebattery.voltage(), corebattery.level());
		} else {
			log_info("Battery at %.2fV (%d%%).", corebattery.voltage(), corebattery.level());
		}
		rtos::ThisThread::sleep_for(1s);
	}
}
