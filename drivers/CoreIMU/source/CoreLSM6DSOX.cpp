// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLSM6DSOX.h"

namespace leka {

CoreLSM6DSOX::CoreLSM6DSOX(interface::I2C &i2c) : _i2c(i2c)
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	_register_io_function.write_reg = reinterpret_cast<stmdev_write_ptr>(ptr_io_write);
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	_register_io_function.read_reg = reinterpret_cast<stmdev_read_ptr>(ptr_io_read);
	_register_io_function.handle   = static_cast<void *>(this);
}

void CoreLSM6DSOX::init()
{
	lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY);
	lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE);
	lsm6dsox_mode_get(&_register_io_function, nullptr, &_config);

	_config.ui.xl.odr = _config.ui.xl.LSM6DSOX_XL_UI_52Hz_HP;
	_config.ui.gy.odr = _config.ui.gy.LSM6DSOX_GY_UI_52Hz_HP;
	_config.ui.xl.fs  = _config.ui.xl.LSM6DSOX_XL_UI_4g;
	_config.ui.gy.fs  = _config.ui.gy.LSM6DSOX_GY_UI_500dps;

	lsm6dsox_mode_set(&_register_io_function, nullptr, &_config);
}

void CoreLSM6DSOX::setPowerMode(PowerMode mode)
{
	if (mode == PowerMode::Off) {
		lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF);
		lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF);
	} else {
		lsm6dsox_xl_hm_mode_t xl_power_mode {};
		lsm6dsox_g_hm_mode_t gy_power_mode {};
		switch (mode) {
			case PowerMode::UltraLow:
				xl_power_mode = LSM6DSOX_ULTRA_LOW_POWER_MD;
				gy_power_mode = LSM6DSOX_GY_NORMAL;
				break;
			case PowerMode::Normal:
				xl_power_mode = LSM6DSOX_LOW_NORMAL_POWER_MD;
				gy_power_mode = LSM6DSOX_GY_NORMAL;
				break;
			case PowerMode::High:
				xl_power_mode = LSM6DSOX_HIGH_PERFORMANCE_MD;
				gy_power_mode = LSM6DSOX_GY_HIGH_PERFORMANCE;
				break;
			default:
				return;
		}
		lsm6dsox_xl_power_mode_set(&_register_io_function, xl_power_mode);
		lsm6dsox_gy_power_mode_set(&_register_io_function, gy_power_mode);
		lsm6dsox_xl_data_rate_set(&_register_io_function, lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_52Hz);
		lsm6dsox_gy_data_rate_set(&_register_io_function, lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_52Hz);
	}
}

auto CoreLSM6DSOX::getData() -> SensorData &
{
	lsm6dsox_data_t data;
	lsm6dsox_data_get(&_register_io_function, nullptr, &_config, &data);
	_sensor_data.xl = {data.ui.xl.mg[0], data.ui.xl.mg[1], data.ui.xl.mg[2]};
	_sensor_data.gy = {data.ui.gy.mdps[0] / 1000, data.ui.gy.mdps[1] / 1000, data.ui.gy.mdps[2] / 1000};
	return _sensor_data;
}

auto CoreLSM6DSOX::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t
{
	// Send component address, without STOP condition
	auto ret = _i2c.write(_address, &register_address, 1, true);
	if (ret == 0) {
		// Read data, with STOP condition
		ret = _i2c.read(_address, p_buffer, number_bytes_to_read, false);
	}

	return ret;
}

auto CoreLSM6DSOX::write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t
{
	if (number_bytes_to_write >= kMaxBufferLength) {
		return 1;
	}

	_rx_buffer[0] = register_address;
	std::copy(p_buffer, (p_buffer + number_bytes_to_write), _rx_buffer.data() + 1);
	auto ret = _i2c.write(_address, _rx_buffer.data(), number_bytes_to_write + 1, false);
	return ret;
}

auto CoreLSM6DSOX::ptr_io_write(CoreLSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer,
								uint16_t number_bytes_to_write) -> int32_t
{
	return handle->write(write_address, number_bytes_to_write, p_buffer);
}

auto CoreLSM6DSOX::ptr_io_read(CoreLSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
							   uint16_t number_bytes_to_read) -> int32_t
{
	return handle->read(read_address, number_bytes_to_read, p_buffer);
}

}	// namespace leka
