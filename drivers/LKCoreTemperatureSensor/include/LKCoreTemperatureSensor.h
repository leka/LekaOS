// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_
#define _LEKA_OS_DRIVER_LK_CORE_TEMPERATURE_SENSOR_H_

#include "mbed.h"
#include <array>

#include "drivers/I2C.h"

#include "internal/HTS221_Driver.h"
#include "internal/LKCoreTemperatureSensorBase.h"

class LKCoreTemperatureSensor : public LKCoreTemperatureSensorDriverBase
{
  public:
	LKCoreTemperatureSensor(I2C &i2C);
	status_t end();
	status_t boot();
	status_t enableIrq();

	status_t getId(uint8_t &id);
	status_t getTemperature(mg_t &);
	status_t getHumidity(mg_t &);

	status_t heaterSet(uint8_t val);
	status_t heaterGet(uint8_t *val);

  private:
	I2C &_i2c;
	uint8_t _address = HTS221_I2C_ADDRESS;

	float_t _humiditySlope, _humidityZero;
	float_t _temperatureSlope, _temperatureZero;

	static const int kBufferSize			 = 32;
	std::array<uint8_t, kBufferSize> _buffer = {0};

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
