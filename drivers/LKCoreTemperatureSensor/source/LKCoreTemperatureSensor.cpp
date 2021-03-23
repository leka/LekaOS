// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

LKCoreTemperatureSensor::LKCoreTemperatureSensor(I2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;

	LKCoreTemperatureSensor::init();
	LKCoreTemperatureSensor::calibration();
}

/**
 * @brief  Initialisation of the HTS221 device
 *
 * @retval interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::init()
{
	// Power on the device
	uint8_t POWER_UP = 0x01;
	if (auto ret = hts221_power_on_set(&_register_io_function, POWER_UP); ret != 0) {
		printf("POWER ON");
		return Status::ERROR;
	}

	uint8_t BDU_ON = 0x01;	 // This feature prevents the reading of LSB and MSB related to different samples.
	if (auto ret = hts221_block_data_update_set(&_register_io_function, BDU_ON); ret != 0) {
		printf("BDU \n");
		return Status::ERROR;
	}

	if (auto ret = hts221_data_rate_set(&_register_io_function, HTS221_ODR_12Hz5); ret != 0) {
		printf("DATA RATE");
		return Status::ERROR;
	}

	uint8_t HEATHER_OFF = 0x00;
	if (auto ret = hts221_heater_set(&_register_io_function, HEATHER_OFF); ret != 0) {
		printf("HEATER \n");
		return Status::ERROR;
	}

	if (auto ret = hts221_temperature_avg_set(&_register_io_function, HTS221_T_AVG_32); ret != 0) {
		printf("TEMP AVG \n");
		return Status::ERROR;
	}

	if (auto ret = hts221_humidity_avg_set(&_register_io_function, HTS221_H_AVG_64); ret != 0) {
		printf("HUM AVG \n");
		return Status::ERROR;
	}

	return Status::SUCCESS;
}

/**
 * @brief  Turn the device off
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::end()
{
	if (auto ret = hts221_power_on_set(&_register_io_function, 0x00); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Boot the device, reloading register with calibration data.
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::boot()
{
	if (auto ret = hts221_boot_set(&_register_io_function, 0x01); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  Enable DRDY that set a signal when data is available
 *
 * @retval         interface status (MANDATORY: Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::enableIrq()
{
	if (auto ret = hts221_pin_mode_set(&_register_io_function, HTS221_OPEN_DRAIN); ret != 0) {	 // Set the pin 3
		return Status::ERROR;
	}

	if (auto ret = hts221_drdy_on_int_set(&_register_io_function, 0x00); ret != 0) {   // DRDY Active when low
		return Status::ERROR;
	}
	return Status::SUCCESS;
}
/**
 * @brief  Compute slope and zero values for calibration.[set]
 *
 * @retval         interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::calibration()
{
	float_t h0rH, h1rH, h0t0Out, h1t0Out, t0degC, t1degC, t0Out, t1Out;
	if (auto ret = hts221_temp_deg_point_0_get(&_register_io_function, &t0degC); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_temp_deg_point_1_get(&_register_io_function, &t1degC); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_temp_adc_point_0_get(&_register_io_function, &t0Out); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_temp_adc_point_1_get(&_register_io_function, &t1Out); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_hum_rh_point_0_get(&_register_io_function, &h0rH); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_hum_rh_point_1_get(&_register_io_function, &h1rH); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_hum_adc_point_0_get(&_register_io_function, &h0t0Out); ret != 0) {
		return Status::ERROR;
	}

	if (auto ret = hts221_hum_adc_point_1_get(&_register_io_function, &h1t0Out); ret != 0) {
		return Status::ERROR;
	}
	_humiditySlope = (h1rH - h0rH) / (2.0 * (h1t0Out - h0t0Out));
	_humidityZero  = h0rH - _humiditySlope * h0t0Out;

	_temperatureSlope = (t1degC - t0degC) / (8.0 * (t1Out - t0Out));
	_temperatureZero  = t0degC - _temperatureSlope * t0Out;

	return Status::SUCCESS;
}

/**
 * @brief  Device Who amI.[get]
 *
 * @param  id     Variable that stores the ID of the device.
 * @retval         interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::getId(uint8_t &id)
{
	if (auto ret = hts221_device_id_get(&_register_io_function, &id); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  DegC temperature.[get]
 *
 * @param  temperatureValue    variable that stores data read
 * @retval         interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::getTemperature(mg_t &temperatureValue)
{
	int16_t rawtemperatureValue;
	if (auto ret = hts221_temperature_raw_get(&_register_io_function, &rawtemperatureValue); ret != 0) {
		return Status::ERROR;
	}
	temperatureValue = rawtemperatureValue * _temperatureSlope + _temperatureZero;
	return Status::SUCCESS;
}

/**
 * @brief  rH Temperature.[get]
 *
 * @param  humidityValue    variable that stores data read
 * @retval         interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::getHumidity(mg_t &humidityValue)
{
	int16_t rawHumidityValue;

	if (auto ret = hts221_humidity_raw_get(&_register_io_function, &rawHumidityValue); ret != 0) {
		return Status::ERROR;
	}

	humidityValue = rawHumidityValue * _humiditySlope + _humidityZero;
	return Status::SUCCESS;
}

/**
 * @brief  State of heater[set]
 *
 * @param 		   val if 0 disable the heater, if 1 turn on the heater
 * @retval         interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::heaterSet(uint8_t val)
{
	if (auto ret = hts221_heater_set(&_register_io_function, val); ret != 0) {
		return Status::ERROR;
	}
	return Status::SUCCESS;
}

/**
 * @brief  State of heater[get]
 *
 * @param 		   val variable where the status of heater is stored
 * @retval         interface status (MANDATORY: return Status::SUCCESS -> no Error)
 *
 */
status_t LKCoreTemperatureSensor::heaterGet(uint8_t *val)
{
	if (auto ret = hts221_heater_get(&_register_io_function, val); ret != 0) {
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
	if (number_bytes_to_write > (kBufferSize - 1)) {
		return 1;
	};

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
