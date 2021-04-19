// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

using namespace leka;

LKCoreTemperatureSensor::LKCoreTemperatureSensor(interface::I2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;
}

void LKCoreTemperatureSensor::init()
{
	powerOnDevice();
	enableBlockDataUpdate();
	setDataAquisitionRate();
	powerOffHeater();
	setAverageTemperature();
	setAverageHumidity();

	calibration();
}

void LKCoreTemperatureSensor::powerOnDevice()
{
	hts221_power_on_set(&_register_io_function, static_cast<uint8_t>(State::ON));
}

void LKCoreTemperatureSensor::enableBlockDataUpdate()
{
	hts221_block_data_update_set(&_register_io_function, static_cast<uint8_t>(State::ON));
}

void LKCoreTemperatureSensor::setDataAquisitionRate()
{
	hts221_data_rate_set(&_register_io_function, HTS221_ODR_7Hz);
}

void LKCoreTemperatureSensor::powerOffHeater()
{
	hts221_heater_set(&_register_io_function, static_cast<uint8_t>(State::OFF));
}

void LKCoreTemperatureSensor::setAverageTemperature()
{
	hts221_temperature_avg_set(&_register_io_function, HTS221_T_AVG_16);
}

void LKCoreTemperatureSensor::setAverageHumidity()
{
	hts221_humidity_avg_set(&_register_io_function, HTS221_H_AVG_32);
}

auto LKCoreTemperatureSensor::getReferenceTemperature()
{
	auto t0degC = getReferenceTemperatureDegC0();
	auto t1degC = getReferenceTemperatureDegC1();
	auto t0adc	= getReferenceTemperatureAdc0();
	auto t1adc	= getReferenceTemperatureAdc1();

	std::tuple<float_t, float_t, float_t, float_t> ref(t0degC, t1degC, t0adc, t1adc);
	return ref;
}

auto LKCoreTemperatureSensor::getReferenceHumidity()
{
	auto h0rh  = getReferenceHumidityRhO();
	auto h1rh  = getReferenceHumidityRh1();
	auto h0adc = getReferenceHumidityAdc0();
	auto h1adc = getReferenceHumidityAdc1();

	std::tuple<float_t, float_t, float_t, float_t> ref(h0rh, h1rh, h0adc, h1adc);
	return ref;
}

void LKCoreTemperatureSensor::calibration()
{
	auto temperature_reference = getReferenceTemperature();
	auto humidity_reference	   = getReferenceHumidity();
	_calibration.temperature =
		leka::utils::linearFunction(std::get<0>(temperature_reference), std::get<1>(temperature_reference),
									std::get<2>(temperature_reference), std::get<3>(temperature_reference));

	_calibration.humidity =
		leka::utils::linearFunction(std::get<0>(humidity_reference), std::get<1>(humidity_reference),
									std::get<2>(humidity_reference), std::get<3>(humidity_reference));
}

LKCoreTemperatureSensor::Calibration const LKCoreTemperatureSensor::getCalibration()
{
	return _calibration;
}

void LKCoreTemperatureSensor::setCalibration(calibration_t sensor_calibration)
{
	_calibration.humidity.slope			 = sensor_calibration.humidity.slope;
	_calibration.humidity.y_intercept	 = sensor_calibration.humidity.y_intercept;
	_calibration.temperature.slope		 = sensor_calibration.temperature.slope;
	_calibration.temperature.y_intercept = sensor_calibration.temperature.y_intercept;
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

float LKCoreTemperatureSensor::getReferenceTemperatureDegC0()
{
	float_t t0degC {0};
	hts221_temp_deg_point_0_get(&_register_io_function, &t0degC);
	return t0degC;
}
float LKCoreTemperatureSensor::getReferenceTemperatureDegC1()
{
	float_t t1degC {0};
	hts221_temp_deg_point_1_get(&_register_io_function, &t1degC);
	return t1degC;
}
float LKCoreTemperatureSensor::getReferenceTemperatureAdc0()
{
	float_t t0Adc {0};
	hts221_temp_adc_point_0_get(&_register_io_function, &t0Adc);
	return t0Adc;
}
float LKCoreTemperatureSensor::getReferenceTemperatureAdc1()
{
	float_t t1Adc {0};
	hts221_temp_adc_point_1_get(&_register_io_function, &t1Adc);
	return t1Adc;
}

float LKCoreTemperatureSensor::getReferenceHumidityRhO()
{
	float_t h0rh {0};
	hts221_hum_rh_point_0_get(&_register_io_function, &h0rh);
	return h0rh;
}

float LKCoreTemperatureSensor::getReferenceHumidityRh1()
{
	float_t h1rh {0};
	hts221_hum_rh_point_1_get(&_register_io_function, &h1rh);
	return h1rh;
}

float LKCoreTemperatureSensor::getReferenceHumidityAdc0()
{
	float_t h0adc {0};
	hts221_hum_adc_point_0_get(&_register_io_function, &h0adc);
	return h0adc;
}

float LKCoreTemperatureSensor::getReferenceHumidityAdc1()
{
	float_t h1adc {0};
	hts221_hum_adc_point_1_get(&_register_io_function, &h1adc);
	return h1adc;
}
