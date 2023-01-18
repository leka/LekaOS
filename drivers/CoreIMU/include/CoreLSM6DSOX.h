// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "CoreEventQueue.h"
#include "CoreInterruptIn.h"
#include "interface/LSM6DSOX.h"
#include "interface/drivers/I2C.h"
#include "lsm6dsox_reg.h"

namespace leka {

class CoreLSM6DSOX : public interface::LSM6DSOX
{
  public:
	explicit CoreLSM6DSOX(interface::I2C &i2c, CoreInterruptIn &drdy_irq);

	void init() final;

	void registerOnGyDataReadyCallback(drdy_callback_t const &callback) final;

	void setPowerMode(PowerMode mode) final;

  private:
	auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t;
	auto write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t;

	static auto ptr_io_write(CoreLSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer,
							 uint16_t number_bytes_to_write) -> int32_t;
	static auto ptr_io_read(CoreLSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
							uint16_t number_bytes_to_read) -> int32_t;

	void onGyrDataReadyHandler();
	void setGyrDataReadyInterrupt();

	interface::I2C &_i2c;
	CoreEventQueue _event_queue {};
	lsm6dsox_md_t _config {};
	stmdev_ctx_t _register_io_function {};
	SensorData _sensor_data {};
	CoreInterruptIn &_drdy_irq;
	const char _address = LSM6DSOX_I2C_ADD_L;

	std::array<int16_t, 3> data_raw_xl {};
	std::array<int16_t, 3> data_raw_gy {};
	drdy_callback_t _on_gy_data_ready_callback;

	static constexpr uint8_t kMaxBufferLength = 32;
	std::array<uint8_t, kMaxBufferLength> _rx_buffer {};
};

}	// namespace leka
