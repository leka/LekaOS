// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "HelloWorld.h"
#include "LKCoreLightSensor.h"

using namespace leka;

HelloWorld hello;

LKCoreLightSensor lightSensor(SENSOR_LIGHT_ADC_INPUT);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		// TODO: print floats
		int length = sprintf(buff, "%d\n", static_cast<int>(100 * lightSensor.readLuminosity()));
		serial.write(buff, length);

		rtos::ThisThread::sleep_for(33ms);
	}

	return 0;
}
