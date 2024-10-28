// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreHTS.h"
#include <cstddef>

using namespace leka;
using namespace leka::utils::math;

CoreHTS::CoreHTS(interface::I2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;
}

void CoreHTS::init()
{
	turnOn();
	turnOffHeater();

	enableBlockDataUpdate();
	setDataAcquisitionRate();

	setNumberOfMeasurementsToCalculateAverageTemperature();
	setNumberOfMeasurementsToCalculateAverageHumidity();

	calibrateTemperature();
	calibrateHumidity();
}

void CoreHTS::turnOn()
{
	hts221_power_on_set(&_register_io_function, 0x01);
}

void CoreHTS::enableBlockDataUpdate()
{
	hts221_block_data_update_set(&_register_io_function, 0x01);
}

void CoreHTS::setDataAcquisitionRate()
{
	hts221_data_rate_set(&_register_io_function, HTS221_ODR_7Hz);
}

void CoreHTS::turnOffHeater()
{
	hts221_heater_set(&_register_io_function, 0x00);
}

void CoreHTS::setNumberOfMeasurementsToCalculateAverageTemperature()
{
	hts221_temperature_avg_set(&_register_io_function, HTS221_T_AVG_16);
}

void CoreHTS::setNumberOfMeasurementsToCalculateAverageHumidity()
{
	hts221_humidity_avg_set(&_register_io_function, HTS221_H_AVG_32);
}

auto CoreHTS::getCalibrationPointsForTemperature() -> std::tuple<Point, Point>
{
	auto status = 0;

	auto p0 = Point {0, 0};
	status += hts221_temp_adc_point_0_get(&_register_io_function, &p0.x);
	status += hts221_temp_deg_point_0_get(&_register_io_function, &p0.y);

	auto p1 = Point {0, 0};
	status += hts221_temp_adc_point_1_get(&_register_io_function, &p1.x);
	status += hts221_temp_deg_point_1_get(&_register_io_function, &p1.y);

	if (status != 0) {
		return std::make_tuple(Point {0, 0}, Point {0, 0});
	}

	return std::make_tuple(p0, p1);
}

auto CoreHTS::getCalibrationPointsForHumidity() -> std::tuple<Point, Point>
{
	auto status = 0;

	auto p0 = Point {0, 0};
	status += hts221_hum_adc_point_0_get(&_register_io_function, &p0.x);
	status += hts221_hum_rh_point_0_get(&_register_io_function, &p0.y);

	auto p1 = Point {0, 0};
	status += hts221_hum_adc_point_1_get(&_register_io_function, &p1.x);
	status += hts221_hum_rh_point_1_get(&_register_io_function, &p1.y);

	if (status != 0) {
		return std::make_tuple(Point {0, 0}, Point {0, 0});
	}

	return std::make_tuple(p0, p1);
}

void CoreHTS::calibrateTemperature()
{
	auto [p0, p1] = getCalibrationPointsForTemperature();

	_temperature_coefficients = computeLinearCoefficients(p0, p1);
}

void CoreHTS::calibrateHumidity()
{
	auto [p0, p1] = getCalibrationPointsForHumidity();

	_humidity_coefficients = computeLinearCoefficients(p0, p1);
}

auto CoreHTS::getTemperatureCalibrationCoefficients() const -> LinearCoefficients
{
	return _temperature_coefficients;
}

auto CoreHTS::getHumidityCalibrationCoefficients() const -> LinearCoefficients
{
	return _humidity_coefficients;
}

void CoreHTS::setLinearCalibrationForTemperature(LinearCoefficients coeffs)
{
	_temperature_coefficients.slope		  = coeffs.slope;
	_temperature_coefficients.y_intercept = coeffs.y_intercept;
}

void CoreHTS::setLinearCalibrationForHumidity(LinearCoefficients coeffs)
{
	_humidity_coefficients.slope	   = coeffs.slope;
	_humidity_coefficients.y_intercept = coeffs.y_intercept;
}

auto CoreHTS::getTemperatureCelsius() -> float
{
	auto raw = int16_t {0};

	if (auto ret = hts221_temperature_raw_get(&_register_io_function, &raw); ret != 0) {
		return -42.0f;
	}

	auto convertFromRaw = [&](auto &value) {
		return static_cast<float>(value) * _temperature_coefficients.slope + _temperature_coefficients.y_intercept;
	};

	return convertFromRaw(raw);
}

auto CoreHTS::getRelativeHumidity() -> float
{
	auto raw = int16_t {0};

	if (auto ret = hts221_humidity_raw_get(&_register_io_function, &raw); ret != 0) {
		return -42.0f;
	}

	auto convertFromRaw = [&](auto &value) {
		return static_cast<float>(value) * _humidity_coefficients.slope + _humidity_coefficients.y_intercept;
	};

	return convertFromRaw(raw);
}

auto CoreHTS::generateCommandWithMultipleAccessToRegister(const uint8_t register_address) const -> uint8_t
{
	auto enable_multiple_access = std::byte {0x80};
	return std::to_integer<uint8_t>(std::byte {register_address} | enable_multiple_access);
}

auto CoreHTS::read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read) -> int
{
	const auto command = generateCommandWithMultipleAccessToRegister(register_address);

	if (auto ret = _i2c.write(_I2C_ADDRESS, &command, 1, true); ret != 0) {
		return ret;
	}

	if (auto ret = _i2c.read(_I2C_ADDRESS, pBuffer, number_bytes_to_read, false); ret != 0) {
		return ret;
	}

	return 0;
}

auto CoreHTS::write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write) -> int
{
	_buffer[0] = generateCommandWithMultipleAccessToRegister(register_address);

	std::copy(pBuffer, (pBuffer + number_bytes_to_write), (_buffer.begin() + 1));

	if (auto ret = _i2c.write(_I2C_ADDRESS, _buffer.data(), (number_bytes_to_write + 1), false); ret != 0) {
		return ret;
	}

	return 0;
}

auto CoreHTS::ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
						   uint16_t number_bytes_to_write) -> int32_t
{
	return (int32_t)((CoreHTS *)handle)->write(register_address, p_buffer, number_bytes_to_write);
}

auto CoreHTS::ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
						  uint16_t number_bytes_to_read) -> int32_t
{
	return (int32_t)((CoreHTS *)handle)->read(register_address, p_buffer, number_bytes_to_read);
}
