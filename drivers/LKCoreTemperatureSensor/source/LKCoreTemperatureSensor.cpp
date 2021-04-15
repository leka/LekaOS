// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

using namespace leka;

LKCoreTemperatureSensor::LKCoreTemperatureSensor(interface::LKCoreI2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;
}

bool LKCoreTemperatureSensor::init()
{
	bool status = false;
	status &= setPower(static_cast<uint8_t>(State::ON));
	status &= setBlock_data_update(static_cast<uint8_t>(State::ON));
	status &= setDataAquisitionRate(HTS221_ODR_7Hz);
	status &= setHeater(static_cast<uint8_t>(State::OFF));
	status &= setAverageTemperature(HTS221_T_AVG_16);
	status &= setAverageHumidity(HTS221_H_AVG_32);

	status &= calibration();

	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::setPower(uint8_t State)
{
	bool status = hts221_power_on_set(&_register_io_function, State);
	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::setBlock_data_update(uint8_t State)
{
	bool status = hts221_block_data_update_set(&_register_io_function, State);
	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::setDataAquisitionRate(hts221_odr_t rate)
{
	bool status = hts221_data_rate_set(&_register_io_function, rate);
	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::setHeater(uint8_t State)
{
	bool status = hts221_heater_set(&_register_io_function, State);
	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::setAverageTemperature(hts221_avgt_t nbAvgTemp)
{
	bool status = hts221_temperature_avg_set(&_register_io_function, nbAvgTemp);
	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::setAverageHumidity(hts221_avgh_t nbAvgHum)
{
	bool status = hts221_humidity_avg_set(&_register_io_function, nbAvgHum);
	return !status;	  // 0 is success for mbed::i2C
}

bool LKCoreTemperatureSensor::getReferenceTemperature(Reference &temperature_reference)
{
	bool status = false;
	status &= hts221_temp_deg_point_0_get(&_register_io_function, &temperature_reference.y0);
	status &= hts221_temp_deg_point_1_get(&_register_io_function, &temperature_reference.y1);
	status &= hts221_temp_adc_point_0_get(&_register_io_function, &temperature_reference.x0);
	status &= hts221_temp_adc_point_1_get(&_register_io_function, &temperature_reference.x1);
	return !status;
}

bool LKCoreTemperatureSensor::getReferenceHumidity(Reference &humidity_reference)
{
	bool status = false;
	status &= hts221_hum_rh_point_0_get(&_register_io_function, &humidity_reference.y0);
	status &= hts221_hum_rh_point_1_get(&_register_io_function, &humidity_reference.y1);
	status &= hts221_hum_adc_point_0_get(&_register_io_function, &humidity_reference.x0);
	status &= hts221_hum_adc_point_1_get(&_register_io_function, &humidity_reference.x1);
	return !status;
}

void LKCoreTemperatureSensor::temperatureCalibration(Reference const &temperature_reference)
{
	_calibration.temperature = utils::LinearInterpolation(temperature_reference.x0, temperature_reference.x1,
														  temperature_reference.y0, temperature_reference.y1);
}

void LKCoreTemperatureSensor::humidityCalibration(Reference const &humidity_reference)
{
	_calibration.humidity = utils::LinearInterpolation(humidity_reference.x0, humidity_reference.x1,
													   humidity_reference.y0, humidity_reference.y1);
}

bool LKCoreTemperatureSensor::calibration()
{
	bool status {0};
	Reference temperature_reference;
	Reference humidity_reference;

	status &= getReferenceTemperature(temperature_reference);
	status &= getReferenceHumidity(humidity_reference);

	temperatureCalibration(temperature_reference);
	humidityCalibration(humidity_reference);

	return !status;
}

LKCoreTemperatureSensor::Calibration const LKCoreTemperatureSensor::getCalibration()
{
	return _calibration;
}
void LKCoreTemperatureSensor::setCalibration(float_t humidity_slope, float_t humidity_y_intercept,
											 float_t temperature_slope, float_t temperature_y_intercept)
{
	_calibration.humidity.slope			 = humidity_slope;
	_calibration.humidity.y_intercept	 = humidity_y_intercept;
	_calibration.temperature.slope		 = temperature_slope;
	_calibration.temperature.y_intercept = temperature_y_intercept;
}

celsius_t LKCoreTemperatureSensor::getTemperature()
{
	int16_t raw_temperature_value;
	float temperature_value = -1;

	hts221_temperature_raw_get(&_register_io_function, &raw_temperature_value);
	temperature_value = raw_temperature_value * _calibration.temperature.slope + _calibration.temperature.y_intercept;

	return temperature_value;
}

relativeHumidity_t LKCoreTemperatureSensor::getHumidity()
{
	int16_t raw_humidity_value;
	float humidity_value = -1;

	hts221_humidity_raw_get(&_register_io_function, &raw_humidity_value);
	humidity_value = raw_humidity_value * _calibration.humidity.slope + _calibration.humidity.y_intercept;

	return humidity_value;
}

int LKCoreTemperatureSensor::read(uint8_t register_command, uint8_t *pBuffer, uint16_t number_bytes_to_read)
{
	uint8_t multiple_access_enable = 0x80;

	uint8_t command = register_command | multiple_access_enable;
	int ret			= _i2c.write(_address, (const char *)&command, 1, true);

	if (ret == 0) {
		ret = _i2c.read(_address, (char *)pBuffer, number_bytes_to_read, false);
	}

	return ret;
}

int LKCoreTemperatureSensor::write(uint8_t register_command, uint8_t *pBuffer, uint16_t number_bytes_to_write)
{
	uint8_t multiple_access_enable = 0x80;

	_buffer[0] = register_command | multiple_access_enable;
	std::copy(pBuffer, (pBuffer + number_bytes_to_write), (_buffer.begin() + 1));

	int ret = _i2c.write(_address, (const char *)_buffer.data(), (number_bytes_to_write + 1), false);

	return ret;
}

int32_t LKCoreTemperatureSensor::ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
											  uint16_t number_bytes_to_write)
{
	return (int32_t)((LKCoreTemperatureSensor *)handle)->write(register_address, p_buffer, number_bytes_to_write);
}

int32_t LKCoreTemperatureSensor::ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
											 uint16_t number_bytes_to_read)
{
	return (int32_t)((LKCoreTemperatureSensor *)handle)->read(register_address, p_buffer, number_bytes_to_read);
}
