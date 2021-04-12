// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

using namespace leka;

/**
 * @brief  Constructor
 */
LKCoreTemperatureSensor::LKCoreTemperatureSensor(interface::LKCoreI2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;
}

/**
 * @brief  Init the device (Power:ON, BDU:ON, AcquisitionRate:7Hz, heater:OFF, AvgTemperature:16, AvgHumidity:32)
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::init()
{
	bool status;
	status &= setPower(state::ON);
	status &= setBDU(state::ON);
	status &= setDataAquisitionRate(HTS221_ODR_7Hz);
	status &= setHeater(state::OFF);
	status &= setAvgTemperature(HTS221_T_AVG_16);
	status &= setAvgHumidity(HTS221_H_AVG_32);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Set the state of the device, 1: ON, 0:OFF
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::setPower(uint8_t state)
{
	auto status = hts221_power_on_set(&_register_io_function, state);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Set the state of the BDU, 1: ON, 0:OFF
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::setBDU(uint8_t state)
// The BDU bit is used to inhibit the output register update between the reading of the
// upper and lower register parts.
{
	auto status = hts221_block_data_update_set(&_register_io_function, state);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Set the data acquisition rate
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::setDataAquisitionRate(hts221_odr_t rate)
{
	auto status = hts221_block_data_update_set(&_register_io_function, rate);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Set heater state
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::setHeater(uint8_t state)
{
	auto status = hts221_heater_set(&_register_io_function, state);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Set the number of temperature measures on which the average is made
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::setAvgTemperature(hts221_avgt_t nbAvgTemp)
{
	auto status = hts221_temperature_avg_set(&_register_io_function, nbAvgTemp);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Set the number of humidity measures on which the average is made
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::setAvgHumidity(hts221_avgh_t nbAvgHum)
{
	auto status = hts221_humidity_avg_set(&_register_io_function, nbAvgHum);
	return !status;	  // 0 is success for mbed::i2C
}

/**
 * @brief  Compute value for slope and y intercept for both temperature and humdity using calibration values stored
 * inside the device
 *
 * @retval         interface status (MANDATORY: true -> no Error)
 */
bool LKCoreTemperatureSensor::calibration()
{
	bool status;
	float_t t0degC;
	status &= hts221_temp_deg_point_0_get(&_register_io_function, &t0degC);

	float_t t1degC;
	status &= hts221_temp_deg_point_1_get(&_register_io_function, &t1degC);

	float_t t0Out;
	status &= hts221_temp_adc_point_0_get(&_register_io_function, &t0Out);

	float_t t1Out;
	status &= hts221_temp_adc_point_1_get(&_register_io_function, &t1Out);

	float_t h0rH;
	status &= hts221_hum_rh_point_0_get(&_register_io_function, &h0rH);

	float_t h1rH;
	status &= hts221_hum_rh_point_1_get(&_register_io_function, &h1rH);

	float_t h0t0Out;
	status &= hts221_hum_adc_point_0_get(&_register_io_function, &h0t0Out);

	float_t h1t0Out;
	status &= hts221_hum_adc_point_1_get(&_register_io_function, &h1t0Out);

	LKCoreTemperatureSensor::_calibration.initialisation = true;

	LKCoreTemperatureSensor::_calibration.humiditySlope = (h1rH - h0rH) / (2.0 * (h1t0Out - h0t0Out));
	LKCoreTemperatureSensor::_calibration.humidity_y_intercept =
		h0rH - LKCoreTemperatureSensor::_calibration.humiditySlope * h0t0Out;

	LKCoreTemperatureSensor::_calibration.temperatureSlope = (t1degC - t0degC) / (8.0 * (t1Out - t0Out));
	LKCoreTemperatureSensor::_calibration.temperature_y_intercept =
		t0degC - LKCoreTemperatureSensor::_calibration.temperatureSlope * t0Out;

	return !status;
}

calibrationValues LKCoreTemperatureSensor::getCalibrationValues()
{
	return _calibration;
}

/**
 * @brief  get DegC temperature.[get]
 *
 * @retval Float Value of temperature in degC
 *
 */
celsius_t LKCoreTemperatureSensor::getTemperature()
{
	int16_t rawtemperatureValue;
	float temperatureValue = -1;

	hts221_temperature_raw_get(&_register_io_function, &rawtemperatureValue);
	temperatureValue = rawtemperatureValue * LKCoreTemperatureSensor::_calibration.temperatureSlope +
					   LKCoreTemperatureSensor::_calibration.temperature_y_intercept;

	return temperatureValue;
}

/**
 * @brief  get humidity level in rH.[get]
 *
 * @retval Float value of humidity in rH
 *
 */
rH_t LKCoreTemperatureSensor::getHumidity()
{
	int16_t rawHumidityValue;
	float humidityValue = -1;

	hts221_humidity_raw_get(&_register_io_function, &rawHumidityValue);
	humidityValue = rawHumidityValue * LKCoreTemperatureSensor::_calibration.humiditySlope +
					LKCoreTemperatureSensor::_calibration.humidity_y_intercept;

	return humidityValue;
}

/**
 * @brief  Read register inside I2C slave device[get]
 *
 * @param 		   register_address command the register to be read
 * @param		   pBuffer buffer that stores data when read
 * @param		   number_bytes_to_read set the number of bytes to read
 * @retval         interface status (MANDATORY: return 0 -> no Error)
 *
 */
int LKCoreTemperatureSensor::read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read)
{
	// Send component address, with no STOP condition
	uint8_t address = register_address | 0x80;
	int ret			= _i2c.write(_address, (const char *)&address, 1, true);
	if (ret == 0) {
		// Read data, with STOP condition
		ret = _i2c.read(_address, (char *)pBuffer, number_bytes_to_read, false);
	}

	return ret;
}

/**
 * @brief  Write to a register inside I2C slave device[set]
 *
 * @param 		   register_address command the register to be read
 * @param		   pBuffer buffer that stores data to be written
 * @param		   number_bytes_to_read set the number of bytes to write
 * @retval         interface status (MANDATORY: return 0 -> no Error)
 *
 */
int LKCoreTemperatureSensor::write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write)
{
	_buffer[0] = register_address | 0x80;	// First, send register address
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
