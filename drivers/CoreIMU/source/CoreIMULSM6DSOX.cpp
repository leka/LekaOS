// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIMULSM6DSOX.h"

namespace leka {

CoreIMULSM6DSOX::CoreIMULSM6DSOX(mbed::I2C &i2c) : _i2c(i2c)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= static_cast<void *>(this);

	init();
}

void CoreIMULSM6DSOX::init()
{
	lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY);
	lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE);
	lsm6dsox_mode_get(&_register_io_function, nullptr, &_config);

	_config.ui.xl.odr = _config.ui.xl.LSM6DSOX_XL_UI_26Hz_HP;
	_config.ui.gy.odr = _config.ui.gy.LSM6DSOX_GY_UI_26Hz_HP;
	_config.ui.xl.fs  = _config.ui.xl.LSM6DSOX_XL_UI_2g;
	_config.ui.gy.fs  = _config.ui.gy.LSM6DSOX_GY_UI_2000dps;

	lsm6dsox_mode_set(&_register_io_function, nullptr, &_config);

	setAccelRate(RateXL::_26Hz);
	setGyrRate(RateGyr::_26Hz);
	setAccelRange(RangeXL::_2G);
	setGyrRange(RangeGyr::_2000dps);
}

void CoreIMULSM6DSOX::setPowerMode(power_mode_t mode)
{
	if (mode == PowerMode::Off) {
		lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF);
	} else {
		lsm6dsox_xl_hm_mode_t power_mode {};
		switch (mode) {
			case PowerMode::UltraLow:
				power_mode = LSM6DSOX_ULTRA_LOW_POWER_MD;
				break;
			case PowerMode::Normal:
				power_mode = LSM6DSOX_LOW_NORMAL_POWER_MD;
				break;
			case PowerMode::High:
				power_mode = LSM6DSOX_HIGH_PERFORMANCE_MD;
				break;
			default:
				return;
		}
		lsm6dsox_xl_power_mode_set(&_register_io_function, power_mode);
	}
}

auto CoreIMULSM6DSOX::getConfig() -> lsm6dsox_md_t &
{
	return _config;
}

void CoreIMULSM6DSOX::updateData()
{
	lsm6dsox_data_t data;
	lsm6dsox_data_get(&_register_io_function, nullptr, &getConfig(), &data);
	_sensors_data.xl.x = data.ui.xl.mg[0];
	_sensors_data.xl.y = data.ui.xl.mg[1];
	_sensors_data.xl.z = data.ui.xl.mg[2];
	_sensors_data.gy.x = data.ui.gy.mdps[0] / 1000;
	_sensors_data.gy.y = data.ui.gy.mdps[1] / 1000;
	_sensors_data.gy.z = data.ui.gy.mdps[2] / 1000;
}

void CoreIMULSM6DSOX::turnOffSensors()
{
	turnOffAccel();
	turnOffGyr();
}

auto CoreIMULSM6DSOX::getAccelX() -> interface::mg_t {
	updateData();
	return _sensors_data.xl.x;
}

 auto CoreIMULSM6DSOX::getAccelY() -> interface::mg_t {
	updateData();
	return _sensors_data.xl.y;
}

 auto CoreIMULSM6DSOX::getAccelZ() -> interface::mg_t {
	updateData();
	return _sensors_data.xl.z;
}

auto CoreIMULSM6DSOX::getAccelXYZ() -> std::tuple<interface::mg_t, interface::mg_t, interface::mg_t>
{
	updateData();
	return std::tuple {_sensors_data.xl.x, _sensors_data.xl.y, _sensors_data.xl.z};
}

void CoreIMULSM6DSOX::setAccelRate(odr_xl_t odr_xl)
{
	lsm6dsox_xl_data_rate_set(&_register_io_function, static_cast<lsm6dsox_odr_xl_t>(odr_xl));
}

void CoreIMULSM6DSOX::setAccelRange(fs_xl_t fs_xl)
{
	lsm6dsox_xl_full_scale_set(&_register_io_function, static_cast<lsm6dsox_fs_xl_t>(fs_xl));
}

auto CoreIMULSM6DSOX::getAccelRate() -> int32_t
{
	lsm6dsox_ctrl1_xl_t reg;
	lsm6dsox_read_reg(&_register_io_function, LSM6DSOX_CTRL1_XL, (uint8_t *)&reg, 1);
	return reg.odr_xl;
}

auto CoreIMULSM6DSOX::getAccelRange() -> int32_t
{
	lsm6dsox_ctrl1_xl_t reg {};
	lsm6dsox_read_reg(&_register_io_function, LSM6DSOX_CTRL1_XL, (uint8_t *)&reg, 1);
	return reg.fs_xl;
}

void CoreIMULSM6DSOX::turnOffAccel()
{
	lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF);
}

auto CoreIMULSM6DSOX::getAngularSpeedX() -> interface::dps_t {
	updateData();
	return _sensors_data.gy.x;
}

auto CoreIMULSM6DSOX::getAngularSpeedY() -> interface::dps_t {
	updateData();
	return _sensors_data.gy.y;
}

auto CoreIMULSM6DSOX::getAngularSpeedZ() -> interface::dps_t {
	updateData();
	return _sensors_data.gy.z;
}

auto CoreIMULSM6DSOX::getAngularSpeedXYZ() -> std::tuple<interface::dps_t, interface::dps_t, interface::dps_t>
{
	updateData();
	return std::tuple {_sensors_data.gy.x, _sensors_data.gy.y, _sensors_data.gy.z};
}

void CoreIMULSM6DSOX::setGyrRate(odr_g_t odr_gy)
{
	lsm6dsox_gy_data_rate_set(&_register_io_function, static_cast<lsm6dsox_odr_g_t>(odr_gy));
}

void CoreIMULSM6DSOX::setGyrRange(fs_g_t fs_gy)
{
	lsm6dsox_gy_full_scale_set(&_register_io_function, static_cast<lsm6dsox_fs_g_t>(fs_gy));
}

auto CoreIMULSM6DSOX::getGyrRate() -> int32_t
{
	lsm6dsox_ctrl2_g_t reg;
	lsm6dsox_read_reg(&_register_io_function, LSM6DSOX_CTRL2_G, (uint8_t *)&reg, 1);
	return reg.odr_g;
}

auto CoreIMULSM6DSOX::getGyrRange() -> int32_t
{
	lsm6dsox_ctrl2_g_t reg;
	lsm6dsox_read_reg(&_register_io_function, LSM6DSOX_CTRL2_G, (uint8_t *)&reg, 1);
	return reg.fs_g;
}

void CoreIMULSM6DSOX::turnOffGyr()
{
	lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF);
}

auto CoreIMULSM6DSOX::getAccelAndAngularSpeed() -> std::tuple<interface::mg_t, interface::mg_t, interface::mg_t,
															  interface::dps_t, interface::dps_t, interface::dps_t>
{
	updateData();
	return std::tuple {_sensors_data.xl.x, _sensors_data.xl.y, _sensors_data.xl.z,
					   _sensors_data.gy.x, _sensors_data.gy.y, _sensors_data.gy.z};
}

auto CoreIMULSM6DSOX::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t
{
	// Send component address, with no STOP condition
	auto ret = _i2c.write(_address, (const char *)&register_address, 1, true);
	if (ret == 0) {
		// Read data, with STOP condition
		ret = _i2c.read(_address, (char *)p_buffer, number_bytes_to_read, false);
	}

	return ret;
}

auto CoreIMULSM6DSOX::write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t
{
	std::array<uint8_t, 32> tmp {};

	if (number_bytes_to_write >= 32) {
		return 2;
	}

	// First, send component address. Then, send data and STOP condition
	tmp[0] = register_address;
	std::copy(p_buffer, (p_buffer + number_bytes_to_write), tmp.data() + 1);
	auto ret = _i2c.write(_address, (const char *)tmp.data(), number_bytes_to_write + 1, false);
	return ret;
}

auto CoreIMULSM6DSOX::ptr_io_write(CoreIMULSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer,
								   uint16_t number_bytes_to_write) -> int32_t
{
	return handle->write(write_address, number_bytes_to_write, p_buffer);
}

auto CoreIMULSM6DSOX::ptr_io_read(CoreIMULSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
								  uint16_t number_bytes_to_read) -> int32_t
{
	return handle->read(read_address, number_bytes_to_read, p_buffer);
}

}	// namespace leka
