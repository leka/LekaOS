// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_IMU_UTILS_H_
#define _LEKA_OS_SPIKE_IMU_UTILS_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"

#include "lsm6dsox_reg.h"
#include "LogKit.h"

namespace leka {
class LKCoreIMU
{
  public:
	LKCoreIMU(mbed::I2C &interface, PinName pin_interrupt);

	void getData(std::array<float, 3> &xl_data, std::array<float, 3> &gy_data);
	auto getId() -> int32_t;

	void setDataRate(lsm6dsox_odr_xl_t odr_xl, lsm6dsox_odr_g_t odr_g);
	auto getDataRate() -> int32_t;

	void TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens);
	void TurnOffSensors();
	void SetFullScale();
	void RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route);
	void print_uint8_t(uint8_t n);

	auto getAllRessources(stmdev_ctx_t *ctx, lsm6dsox_all_sources_t *val) -> int32_t;

	auto get_reg_io_function() -> stmdev_ctx_t& { return _register_io_function;}

	auto WriteReg(stmdev_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t len) 	-> int32_t;
	auto setBlockDataUpdate(stmdev_ctx_t *ctx, uint8_t val) 					-> int32_t;
	auto setIntNotification(stmdev_ctx_t *ctx, lsm6dsox_lir_t val)				-> int32_t;
	auto setEmbeddedSens(stmdev_ctx_t *ctx, lsm6dsox_emb_sens_t *val)			-> int32_t;
	auto setMLCDataRate(stmdev_ctx_t *ctx, lsm6dsox_mlc_odr_t val) 				-> int32_t;
	auto getMLCOut(stmdev_ctx_t *ctx, uint8_t *buff)							-> int32_t;

	static auto ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer, uint16_t number_bytes_to_write) -> int32_t;
	static auto ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read) -> int32_t;

  private:
	auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t;
	auto write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t;

	mbed::I2C &_interface;
	uint8_t _address = LSM6DSOX_I2C_ADD_L;
	PinName _lsm6dsox_interrupt;

	stmdev_ctx_t _register_io_function;
	lsm6dsox_md_t _config;
};
}	// namespace leka


static void bytecpy(uint8_t *target, uint8_t *source);

#endif	 // _LEKA_OS_SPIKE_IMU_UTILS_H_
