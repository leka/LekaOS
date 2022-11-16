// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "interface/LSM6DSOX.h"
#include "interface/drivers/I2C.h"
#include "lsm6dsox_reg.h"

namespace leka {

class CoreLSM6DSOX : public interface::LSM6DSOX
{
  public:
	explicit CoreLSM6DSOX(interface::I2C &i2c);

	void init() final;
	auto getData() -> SensorData & final;
	void setPowerMode(PowerMode mode) final;

  private:
	auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t;
	auto write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t;

	static auto ptr_io_write(CoreLSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer,
							 uint16_t number_bytes_to_write) -> int32_t;
	static auto ptr_io_read(CoreLSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
							uint16_t number_bytes_to_read) -> int32_t;

	interface::I2C &_i2c;
	lsm6dsox_md_t _config {};
	stmdev_ctx_t _register_io_function {};
	SensorData _sensor_data {};
	const char _address = LSM6DSOX_I2C_ADD_L;

	static constexpr uint8_t kMaxBufferLength = 32;
	std::array<uint8_t, kMaxBufferLength> _rx_buffer {};
};

}	// namespace leka
