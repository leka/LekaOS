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
	printf("\nHello, Investigation Day!\n\n");

	if (temperatureSensor.boot() != Status::SUCCESS) {
		printf("Boot failed \n");
	}

	if (temperatureSensor.enableIrq() != Status::SUCCESS) {
		printf("Enable Irq failed\n");
	}

	if (temperatureSensor.heaterSet(state::ON) != Status::SUCCESS) {
		printf("Heater On failed\n");
	}

	uint8_t heater = temperatureSensor.heaterGet();
	printf("Value of heater : %i\n", heater);

	if (temperatureSensor.heaterSet(state::OFF) != Status::SUCCESS) {
		printf("Heater Off failed\n");
	}

	uint8_t id = temperatureSensor.getId();
	printf("Device ID : %i\n", id);

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		float temperature = temperatureSensor.getTemperature();
		printf("Value of temperature : %f\n", temperature);

		float humidity = temperatureSensor.getHumidity();
		printf("Value of humidity : %f\n", humidity);

		rtos::ThisThread::sleep_for(5s);
	}

	// if (temperatureSensor.turnOff() != Status::SUCCESS) {
	// 	printf("End failed\n");
	// }
}
