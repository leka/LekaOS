// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/I2C.h"

#include "interface/Accelerometer.h"
#include "interface/Gyroscope.h"

namespace leka {

class CoreIMULSM6DSOX : public interface::Accelerometer, public interface::Gyroscope
{
  public:
	explicit CoreIMULSM6DSOX(mbed::I2C &i2c);

	enum class PowerMode
	{
		Off,
		UltraLow,
		Normal,
		High,
	};

	struct SensorsData {
		accel_data_t xl;
		gyro_data_t gy;
	};

	struct ucf_line_t {
		uint8_t address;
		uint8_t data;
	};

	using power_mode_t = PowerMode;

	void init();
	void setPowerMode(power_mode_t);
	auto getConfig() -> lsm6dsox_md_t &;
	void updateData();
	void turnOffSensors();

	auto getAccelX() -> interface::mg_t final;
	auto getAccelY() -> interface::mg_t final;
	auto getAccelZ() -> interface::mg_t final;
	auto getAccelXYZ() -> std::tuple<interface::mg_t, interface::mg_t, interface::mg_t> final;
	void setAccelRate(odr_xl_t odr_xl) final;
	void setAccelRange(fs_xl_t fs_xl) final;
	auto getAccelRate() -> int32_t final;
	auto getAccelRange() -> int32_t final;
	void turnOffAccel() final;

	auto getAngularSpeedX() -> interface::dps_t final;
	auto getAngularSpeedY() -> interface::dps_t final;
	auto getAngularSpeedZ() -> interface::dps_t final;
	auto getAngularSpeedXYZ() -> std::tuple<interface::dps_t, interface::dps_t, interface::dps_t> final;
	void setGyrRate(odr_g_t odr_gy) final;
	void setGyrRange(fs_g_t fs_gy) final;
	auto getGyrRate() -> int32_t final;
	auto getGyrRange() -> int32_t final;
	void turnOffGyr() final;

	auto getAccelAndAngularSpeed() -> std::tuple<interface::mg_t, interface::mg_t, interface::mg_t, interface::dps_t,
												 interface::dps_t, interface::dps_t>;

  private:
	auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t;
	auto write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t;

	static auto ptr_io_write(CoreIMULSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer,
							 uint16_t number_bytes_to_write) -> int32_t;
	static auto ptr_io_read(CoreIMULSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
							uint16_t number_bytes_to_read) -> int32_t;

	mbed::I2C &_i2c;
	lsm6dsox_md_t _config {};
	SensorsData _sensors_data {};
	stmdev_ctx_t _register_io_function {};
	const char _address = LSM6DSOX_I2C_ADD_L;
};

}	// namespace leka
