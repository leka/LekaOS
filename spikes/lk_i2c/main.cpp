// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "HelloWorld.h"
#include "LKCoreTemperatureSensor.h"

using namespace leka;

HelloWorld hello;

mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreTemperatureSensor temperatureSensor(i2c);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	if (temperatureSensor.boot() != Status::SUCCESS) {
		printf("Boot failed \n");
	}

	if (temperatureSensor.enableIrq() != Status::SUCCESS) {
		printf("Enable Irq failed\n");
	}

	if (temperatureSensor.heaterSet(0x01) != Status::SUCCESS) {
		printf("Heater On failed\n");
	}

	uint8_t heaterState;
	if (temperatureSensor.heaterGet(&heaterState) != Status::SUCCESS) {
		printf("Calibration failed\n");
	} else {
		printf("Value of heater : %i\n", heaterState);
	}

	if (temperatureSensor.heaterSet(0x00) != Status::SUCCESS) {
		printf("Heater Off failed\n");
	}

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		auto t = Kernel::Clock::now() - start;
		uint8_t id;
		if (temperatureSensor.getId(id) != Status::SUCCESS) {
			printf("Failed to read ID.\n");
		}

		float temperature;
		temperatureSensor.getTemperature(temperature);
		printf("Value of temperature : %f\n", temperature);

		float humidity;
		temperatureSensor.getHumidity(humidity);
		printf("Value of humidity : %f\n", humidity);

		// if (temperatureSensor.end() != Status::SUCCESS) {
		// 	printf("End failed\n");
		// }
		rtos::ThisThread::sleep_for(5s);
	}
	return 0;
}
