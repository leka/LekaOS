// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "HelloWorld.h"
#include "I2C.h"
#include "LKCoreTemperatureSensor.h"

using namespace leka;

int main(void)
{
	HelloWorld hello;

	I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	LKCoreI2C interface_temperature_sensor(i2c);
	LKCoreTemperatureSensor temperature_sensor(interface_temperature_sensor);

	rtos::ThisThread::sleep_for(2s);

	hello.start();
	temperature_sensor.init();
	LKCoreTemperatureSensor::Calibration calibration = temperature_sensor.getCalibration();

	printf(
		"Calibration values : \n is_initialise: %i,\n humidity_slope: %f,\n humidity_y_intercept: "
		"%f,\n temperature_slope: %f,\n temperature_y_intercept: %f\n\n",
		calibration.is_initialise, calibration.humidity.slope, calibration.humidity.y_intercept,
		calibration.temperature.slope, calibration.temperature.y_intercept);

	while (true) {
		auto temperature = temperature_sensor.getTemperature();
		auto humidity	 = temperature_sensor.getHumidity();

		printf("Temperature : %f degC, Humidite : %f rH\n", temperature, humidity);

		rtos::ThisThread::sleep_for(1s);
	}
}
