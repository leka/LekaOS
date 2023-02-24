// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreQDAC.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

//
// MARK: - Global definitions
//

auto corei2c = CoreI2C {PinName::SENSOR_PROXIMITY_MUX_I2C_SDA, PinName::SENSOR_PROXIMITY_MUX_I2C_SCL};
auto dac	 = CoreQDACMCP4728 {corei2c, 0xC2};

auto data = uint16_t {};

void printInputRegisters()
{
	// for (auto channels = std::array<uint8_t, 4> {mcp4728::channel::A, mcp4728::channel::B, mcp4728::channel::C,
	// 											 mcp4728::channel::D};
	// 	 uint8_t channel: channels) {
	// 	log_info("================= Channel %d ================= ", channel);
	// 	data = dac.read(channel);
	// 	log_info("Input Registers === Data: %x", data);
	// 	log_info("\n\n");
	// }
	// log_info("================= Channel %d ================= ", 0);
	data = dac.read(2);
	log_info("Input Registers === Data: %x", data);
	log_info("\n\n");
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	rtos::ThisThread::sleep_for(1s);

	auto channel = uint8_t {};
	log_info("Read DAC (channel 0=A, 1=B, 2=C, 3=D):\n");

	log_info("Init !\n");
	dac.init();
	rtos::ThisThread::sleep_for(100ms);
	printInputRegisters();
	rtos::ThisThread::sleep_for(2s);

	channel = 0x02;

	data = 0x0ABD;
	log_info("Writing ! Channel : %d, Data : %x\n", channel, data);
	dac.write(channel, data);
	rtos::ThisThread::sleep_for(100ms);
	printInputRegisters();
	rtos::ThisThread::sleep_for(1s);

	// channel = 0x01;

	// data = 0x0DEF;
	// log_info("Writing ! Channel : %d, Data : %x\n", channel, data);
	// dac.write(channel, data);
	// rtos::ThisThread::sleep_for(100ms);
	// printInputRegisters();
	// rtos::ThisThread::sleep_for(1s);
}
