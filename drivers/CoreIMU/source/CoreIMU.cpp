// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIMU.hpp"

namespace leka {

CoreIMU::CoreIMU(interface::I2C &i2c, CoreInterruptIn &irq) : _i2c(i2c), _irq(irq)
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	_register_io_function.write_reg = reinterpret_cast<stmdev_write_ptr>(ptr_io_write);
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	_register_io_function.read_reg = reinterpret_cast<stmdev_read_ptr>(ptr_io_read);
	_register_io_function.handle   = static_cast<void *>(this);
}

void CoreIMU::init()
{
	_event_queue.dispatch_forever();

	lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY);
	lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE);
	lsm6dsox_mode_get(&_register_io_function, nullptr, &_config);

	_config.ui.xl.odr = _config.ui.xl.LSM6DSOX_XL_UI_52Hz_HP;
	_config.ui.gy.odr = _config.ui.gy.LSM6DSOX_GY_UI_52Hz_HP;
	_config.ui.xl.fs  = _config.ui.xl.LSM6DSOX_XL_UI_4g;
	_config.ui.gy.fs  = _config.ui.gy.LSM6DSOX_GY_UI_500dps;

	lsm6dsox_mode_set(&_register_io_function, nullptr, &_config);

	lsm6dsox_dataready_pulsed_t data_ready_pulsed {LSM6DSOX_DRDY_PULSED};
	lsm6dsox_data_ready_mode_set(&_register_io_function, data_ready_pulsed);

	enableOnDataReadyInterrupt();
}

void CoreIMU::setPowerMode(PowerMode mode)
{
	auto xl_power_mode = lsm6dsox_xl_hm_mode_t {};
	auto gy_power_mode = lsm6dsox_g_hm_mode_t {};

	auto xl_odr = lsm6dsox_odr_xl_t {LSM6DSOX_XL_ODR_52Hz};
	auto gy_odr = lsm6dsox_odr_g_t {LSM6DSOX_GY_ODR_52Hz};

	switch (mode) {
		case PowerMode::Off:
			xl_odr		  = LSM6DSOX_XL_ODR_OFF;
			gy_odr		  = LSM6DSOX_GY_ODR_OFF;
			xl_power_mode = LSM6DSOX_ULTRA_LOW_POWER_MD;
			gy_power_mode = LSM6DSOX_GY_NORMAL;
			break;

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
	}

	lsm6dsox_xl_power_mode_set(&_register_io_function, xl_power_mode);
	lsm6dsox_gy_power_mode_set(&_register_io_function, gy_power_mode);
	lsm6dsox_xl_data_rate_set(&_register_io_function, xl_odr);
	lsm6dsox_gy_data_rate_set(&_register_io_function, gy_odr);
}

void CoreIMU::registerOnDataReadyCallback(data_ready_callback_t const &callback)
{
	_on_data_ready_callback = callback;
}

void CoreIMU::onDataReadyHandler(auto timestamp)
{
	static constexpr auto _1k = float {1000.F};

	_sensor_data = SensorData {.timestamp = timestamp};

	lsm6dsox_angular_rate_raw_get(&_register_io_function, data_raw_gy.data());
	_sensor_data.gy.x = lsm6dsox_from_fs500_to_mdps(data_raw_gy.at(0)) / _1k;
	_sensor_data.gy.y = lsm6dsox_from_fs500_to_mdps(data_raw_gy.at(1)) / _1k;
	_sensor_data.gy.z = lsm6dsox_from_fs500_to_mdps(data_raw_gy.at(2)) / _1k;

	lsm6dsox_acceleration_raw_get(&_register_io_function, data_raw_xl.data());
	_sensor_data.xl.x = lsm6dsox_from_fs4_to_mg(data_raw_xl.at(0)) / _1k;
	_sensor_data.xl.y = lsm6dsox_from_fs4_to_mg(data_raw_xl.at(1)) / _1k;
	_sensor_data.xl.z = lsm6dsox_from_fs4_to_mg(data_raw_xl.at(2)) / _1k;

	if (_on_data_ready_callback) {
		_on_data_ready_callback(_sensor_data);
	}
}

void CoreIMU::enableOnDataReadyInterrupt()
{
	lsm6dsox_pin_int1_route_t lsm6dsox_int1 {
		.drdy_xl  = PROPERTY_ENABLE,
		.den_flag = PROPERTY_ENABLE,
	};
	lsm6dsox_pin_int1_route_set(&_register_io_function, lsm6dsox_int1);

	auto on_data_ready_callback = [this] {
		auto timestamp = rtos::Kernel::Clock::now();
		_event_queue.call([this, timestamp] { onDataReadyHandler(timestamp); });
	};

	setDataReadyInterruptCallback(on_data_ready_callback);
}

void CoreIMU::disableOnDataReadyInterrupt()
{
	lsm6dsox_pin_int1_route_t lsm6dsox_int1 {
		.drdy_xl  = PROPERTY_DISABLE,
		.den_flag = PROPERTY_DISABLE,
	};
	lsm6dsox_pin_int1_route_set(&_register_io_function, lsm6dsox_int1);

	setDataReadyInterruptCallback({});
}

void CoreIMU::enableDeepSleep()
{
	setPowerMode(interface::IMU::PowerMode::Off);
}

void CoreIMU::disableDeepSleep()
{
	setPowerMode(interface::IMU::PowerMode::Normal);
}

auto CoreIMU::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t
{
	// Send component address, without STOP condition
	auto ret = _i2c.write(_address, &register_address, 1, true);
	if (ret == 0) {
		// Read data, with STOP condition
		ret = _i2c.read(_address, p_buffer, number_bytes_to_read, false);
	}

	return ret;
}

auto CoreIMU::write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t
{
	if (number_bytes_to_write >= kMaxBufferLength) {
		return 1;
	}

	_rx_buffer[0] = register_address;
	std::copy(p_buffer, (p_buffer + number_bytes_to_write), _rx_buffer.data() + 1);
	auto ret = _i2c.write(_address, _rx_buffer.data(), number_bytes_to_write + 1, false);
	return ret;
}

auto CoreIMU::ptr_io_write(CoreIMU *handle, uint8_t write_address, uint8_t *p_buffer,
						   uint16_t number_bytes_to_write) -> int32_t
{
	return handle->write(write_address, number_bytes_to_write, p_buffer);
}

auto CoreIMU::ptr_io_read(CoreIMU *handle, uint8_t read_address, uint8_t *p_buffer,
						  uint16_t number_bytes_to_read) -> int32_t
{
	return handle->read(read_address, number_bytes_to_read, p_buffer);
}

void CoreIMU::setDataReadyInterruptCallback(std::function<void()> const &callback)
{
	if (callback) {
		_irq.onRise(callback);
	}
}

}	// namespace leka
