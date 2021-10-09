// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "HelloWorld.h"
#include "LKCoreLightSensor.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_trace_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	LKCoreLightSensor lightSensor(SENSOR_LIGHT_ADC_INPUT);

	while (true) {
		log_info("Current luminosity: %.4f", lightSensor.readLuminosity());
		rtos::ThisThread::sleep_for(33ms);
	}
}
