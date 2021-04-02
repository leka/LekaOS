// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_

#include <array>

#include "drivers/I2C.h"

#include "internal/HTS221_Driver.h"
#include "internal/LKCoreTemperatureSensorBase.h"

using namespace leka;

// namespace HeaterState {
// uint8_t HEATER_ON  = 1;
// uint8_t HEATER_OFF = 0;

// };	 // namespace HeaterState

class LKCoreTemperatureSensor : public LKCoreTemperatureSensorDriverBase
{
  public:
	LKCoreTemperatureSensor(mbed::I2C &i2C);
	status_t turnOn();
	status_t turnOff();
	status_t boot();
	status_t enableIrq();

	uint8_t getId();
	celsius_t getTemperature();
	rH_t getHumidity();

	status_t heaterSet(uint8_t val);
	uint8_t heaterGet();

  private:
	mbed::I2C &_i2c;
	uint8_t _address = HTS221_I2C_ADDRESS;

	float_t _humiditySlope {0};
	float_t _humidity_y_intercept {0};
	float_t _temperatureSlope {0};
	float_t _temperature_y_intercept {0};

	std::array<uint8_t, 32> _buffer = {0};

	status_t init();
	status_t calibration();

	int read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read);
	int write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write);

	stmdev_ctx_t _register_io_function;
	static int32_t ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
								uint16_t number_bytes_to_write);
	static int32_t ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
							   uint16_t number_bytes_to_read);
};

#endif	 // _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_
