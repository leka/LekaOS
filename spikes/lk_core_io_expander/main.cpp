// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreI2C.h"
#include "CoreIOExpander.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto corei2c		   = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
auto io_expander	   = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	while (true) {
		auto pin = mcp23017::pin::PA5;
		auto val = io_expander.readPin(pin);
		log_info("Pin %d : state = %s", pin, (val == 1) ? "true" : "false");
		rtos::ThisThread::sleep_for(100ms);
	}
}
