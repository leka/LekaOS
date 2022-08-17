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
#include "IOKit/DigitalIn.h"
#include "IOKit/DigitalOut.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto corei2c		   = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
auto io_expander_reset = mbed::DigitalOut {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
auto io_expander	   = CoreIOExpanderMCP23017 {corei2c, io_expander_reset};

auto in	 = io::expanded::DigitalIn<> {io_expander, mcp23017::pin::PB5};
auto out = io::expanded::DigitalOut<> {io_expander, mcp23017::pin::PA5};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	auto input_state  = bool {};
	auto output_state = bool {};
	out.write(mcp23017::level::high);

	while (true) {
		in.mode(PinMode::PullUp);
		log_info("Input : mode = Pull Up !");
		input_state = (1 == in.read());
		log_info("Input : state = %s", input_state ? "true" : "false");
		rtos::ThisThread::sleep_for(100ms);

		rtos::ThisThread::sleep_for(1s);
	}
}
