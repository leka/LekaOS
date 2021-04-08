// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreHTS221Driver.h"

using namespace leka;

/**
 * @brief  Constructor
 */
LKCoreHTS221Driver::LKCoreHTS221Driver(mbed::I2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;
}

/**
 * @brief  Init the device (Power:ON, BDU:ON, AcquisitionRate:7Hz, heater:OFF, AvgTemperature:16, AvgHumidity:32)
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::init()
{
	// Power on the device
	if (auto ret = setPower(state::ON); ret != Status::SUCCESS) {
		return Status::ERROR;
	}

	if (auto ret = setBDU(state::ON); ret != Status::SUCCESS) {
		return Status::ERROR;
	}

	if (auto ret = setDataAquisitionRate(HTS221_ODR_7Hz); ret != Status::SUCCESS) {
		return Status::ERROR;
	}

	if (auto ret = setHeater(state::OFF); ret != Status::SUCCESS) {
		return Status::ERROR;
	}

	if (auto ret = setAvgTemperature(HTS221_T_AVG_16); ret != Status::SUCCESS) {
		return Status::ERROR;
	}

	if (auto ret = setAvgHumidity(HTS221_H_AVG_32); ret != Status::SUCCESS) {
		return Status::ERROR;
	}

	return Status::SUCCESS;
}

/**
 * @brief  Set the state of the device, 1: ON, 0:OFF
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setPower(uint8_t state)
{
	if (auto ret = hts221_power_on_set(&_register_io_function, state); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Set the state of the BDU, 1: ON, 0:OFF
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setBDU(uint8_t state)
// The BDU bit is used to inhibit the output register update between the reading of the
// upper and lower register parts.
{
	if (auto ret = hts221_block_data_update_set(&_register_io_function, state); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Set the data acquisition rate
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setDataAquisitionRate(hts221_odr_t rate)
{
	if (auto ret = hts221_block_data_update_set(&_register_io_function, rate); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Set the number of temperature measures on which the average is made
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setAvgTemperature(hts221_avgt_t nbAvgTemp)
{
	if (auto ret = hts221_temperature_avg_set(&_register_io_function, nbAvgTemp); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Set the number of humidity measures on which the average is made
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setAvgHumidity(hts221_avgh_t nbAvgHum)
{
	if (auto ret = hts221_humidity_avg_set(&_register_io_function, nbAvgHum); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Compute value for slope and y intercept for both temperature and humdity using calibration values stored
 * inside the device
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::calibration()
{
	float_t t0degC;
	if (auto ret = hts221_temp_deg_point_0_get(&_register_io_function, &t0degC); ret != 0) {
		return Status::ERROR;
	}

	float_t t1degC;
	if (auto ret = hts221_temp_deg_point_1_get(&_register_io_function, &t1degC); ret != 0) {
		return Status::ERROR;
	}

	float_t t0Out;
	if (auto ret = hts221_temp_adc_point_0_get(&_register_io_function, &t0Out); ret != 0) {
		return Status::ERROR;
	}

	float_t t1Out;
	if (auto ret = hts221_temp_adc_point_1_get(&_register_io_function, &t1Out); ret != 0) {
		return Status::ERROR;
	}

	float_t h0rH;
	if (auto ret = hts221_hum_rh_point_0_get(&_register_io_function, &h0rH); ret != 0) {
		return Status::ERROR;
	}

	float_t h1rH;
	if (auto ret = hts221_hum_rh_point_1_get(&_register_io_function, &h1rH); ret != 0) {
		return Status::ERROR;
	}

	float_t h0t0Out;
	if (auto ret = hts221_hum_adc_point_0_get(&_register_io_function, &h0t0Out); ret != 0) {
		return Status::ERROR;
	}

	float_t h1t0Out;
	if (auto ret = hts221_hum_adc_point_1_get(&_register_io_function, &h1t0Out); ret != 0) {
		return Status::ERROR;
	}

	_humiditySlope		  = (h1rH - h0rH) / (2.0 * (h1t0Out - h0t0Out));
	_humidity_y_intercept = h0rH - _humiditySlope * h0t0Out;

	_temperatureSlope		 = (t1degC - t0degC) / (8.0 * (t1Out - t0Out));
	_temperature_y_intercept = t0degC - _temperatureSlope * t0Out;

	return Status::SUCCESS;
}

/**
 * @brief  Return the id of the device
 *
 * @retval        uint8_t
 */
uint8_t LKCoreHTS221Driver::getId()
{
	uint8_t id = 0;
	hts221_device_id_get(&_register_io_function, &id);
	return id;
}

/**
 * @brief  Return the raw value of temperature
 *
 * @retval         int16_t
 */
int16_t LKCoreHTS221Driver::getRawTemperature()
{
	int16_t rawTemperaturevalue = 0;
	hts221_temperature_raw_get(&_register_io_function, &rawTemperaturevalue);
	return rawTemperaturevalue;
}

/**
 * @brief  Return the raw value of humidity
 *
 * @retval         int16_t
 */
int16_t LKCoreHTS221Driver::getRawHumidity()
{
	int16_t rawHumidityvalue = 0;
	hts221_temperature_raw_get(&_register_io_function, &rawHumidityvalue);
	return rawHumidityvalue;
}

/**
 * @brief  Set the state of the heater, 1:ON, 0:OFF
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setHeater(uint8_t state)
{
	if (auto ret = hts221_heater_set(&_register_io_function, state); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Get the state of the heater
 *
 * @retval         uint8_t
 */
uint8_t LKCoreHTS221Driver::getHeater()
{
	uint8_t heaterState = 0;
	hts221_heater_get(&_register_io_function, &heaterState);
	return heaterState;
}

/**
 * @brief  Set DRDY state, if 1: set a signal when data is available
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 */
status_t LKCoreHTS221Driver::setIrq(uint8_t state)
{
	if (auto ret = hts221_pin_mode_set(&_register_io_function, HTS221_OPEN_DRAIN); ret != 0) {	 // Set the pin 3
		return Status::ERROR;
	}

	if (auto ret = hts221_drdy_on_int_set(&_register_io_function, state); ret != 0) {	// DRDY Active when low
		return Status::ERROR;
	}
	return Status::SUCCESS;
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
int LKCoreHTS221Driver::read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read)
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
int LKCoreHTS221Driver::write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write)
{
	if (number_bytes_to_write > (_buffer.size() - 1)) {
		return 1;
	}

	_buffer[0] = register_address | 0x80;	// First, send register address
	std::copy(pBuffer, (pBuffer + number_bytes_to_write), (_buffer.begin() + 1));

	int ret = _i2c.write(_address, (const char *)_buffer.data(), (number_bytes_to_write + 1), false);

	return ret;
}

int32_t LKCoreHTS221Driver::ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
										 uint16_t number_bytes_to_write)
{
	return (int32_t)((LKCoreHTS221Driver *)handle)->write(register_address, p_buffer, number_bytes_to_write);
}

int32_t LKCoreHTS221Driver::ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
										uint16_t number_bytes_to_read)
{
	return (int32_t)((LKCoreHTS221Driver *)handle)->read(register_address, p_buffer, number_bytes_to_read);
}
