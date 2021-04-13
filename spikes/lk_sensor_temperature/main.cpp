// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "HelloWorld.h"
#include "I2C.h"
#include "LKCoreTemperatureSensor.h"

using namespace leka;

HelloWorld hello;

I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
LKCoreI2C interfaceTemperatureSensor(i2c);
LKCoreTemperatureSensor temperatureSensor(interfaceTemperatureSensor);

static BufferedSerial serial(USBTX, USBRX, 9600);
constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();
	rtos::ThisThread::sleep_for(2s);

	hello.start();
	temperatureSensor.init();
	temperatureSensor.calibration();
	calibrationValues calibrationValuesSpike = temperatureSensor.getCalibrationValues();

	printf("Calibration : %i, %f, %f, %f, %f \n", calibrationValuesSpike.initialisation,
		   calibrationValuesSpike.humiditySlope, calibrationValuesSpike.humidity_y_intercept,
		   calibrationValuesSpike.temperatureSlope, calibrationValuesSpike.temperature_y_intercept);

	while (true) {
		auto t = Kernel::Clock::now() - start;

		auto temperature = temperatureSensor.getTemperature();
		auto humidity	 = temperatureSensor.getHumidity();

		int length = sprintf(buff, "%f, %f\n", temperature, humidity);
		serial.write(buff, length);

		rtos::ThisThread::sleep_for(1s);
	}
}
