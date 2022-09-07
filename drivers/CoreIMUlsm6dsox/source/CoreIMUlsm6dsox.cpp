// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIMUlsm6dsox.h"

namespace leka {

CoreIMU_LSM6DSOX::CoreIMU_LSM6DSOX(mbed::I2C &interface, PinName pin_interrupt)
	: _interface(interface), _lsm6dsox_interrupt(pin_interrupt)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;

	mbed::DigitalOut INT_1_LSM6DSOX(_lsm6dsox_interrupt, 0);

	lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY);
	lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE);

	lsm6dsox_mode_get(&_register_io_function, nullptr, &_config);
}

auto CoreIMU_LSM6DSOX::ptr_io_write(CoreIMU_LSM6DSOX *handle, uint8_t write_address, const uint8_t *p_buffer,
									uint16_t number_bytes_to_write) -> int32_t
{
	return handle->write(write_address, number_bytes_to_write, p_buffer);
}

auto CoreIMU_LSM6DSOX::ptr_io_read(CoreIMU_LSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
								   uint16_t number_bytes_to_read) -> int32_t
{
	return handle->read(read_address, number_bytes_to_read, p_buffer);
}

auto CoreIMU_LSM6DSOX::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *pBuffer) -> int32_t
{
	int ret;

	/* Send component address, with no STOP condition */
	ret = _interface.write(_address, (const char *)&register_address, 1, true);
	if (!ret) {
		/* Read data, with STOP condition  */
		ret = _interface.read(_address, (char *)pBuffer, number_bytes_to_read, false);
	}

	if (ret) return 1;
	return 0;
}

auto CoreIMU_LSM6DSOX::write(uint8_t register_address, uint16_t number_bytes_to_write, const uint8_t *pBuffer) -> int32_t
{
	int ret;
	std::array<uint8_t, 32> tmp;

	if (number_bytes_to_write >= 32) return 2;

	/* First, send component address. Then, send data and STOP condition */
	tmp[0] = register_address;
	std::copy(pBuffer, pBuffer + number_bytes_to_write, tmp.data() + 1);
	ret = _interface.write(_address, (const char *)tmp.data(), number_bytes_to_write + 1, false);

	if (ret) return 1;
	return 0;
}

// Return 0 in case of success
auto CoreIMU_LSM6DSOX::getId() -> int32_t
{
	lsm6dsox_id_t val;
	if (lsm6dsox_id_get(&_register_io_function, nullptr, &val) != 0)
		return 1;
	else
		return 0;
}

auto CoreIMU_LSM6DSOX::setMLCDataRate(stmdev_ctx_t *ctx, lsm6dsox_mlc_odr_t val) const -> int32_t
{
	return lsm6dsox_mlc_data_rate_set(ctx, val);
}

void CoreIMU_LSM6DSOX::TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens)
{
	lsm6dsox_embedded_sens_get(&_register_io_function, emb_sens);
	lsm6dsox_embedded_sens_off(&_register_io_function);
}

void CoreIMU_LSM6DSOX::TurnOffSensors()
{
	lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF);
	lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF);
}

void CoreIMU_LSM6DSOX::RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route)
{
	lsm6dsox_pin_int1_route_get(&_register_io_function, pin_int1_route);
	pin_int1_route->mlc1 = PROPERTY_ENABLE;
	lsm6dsox_pin_int1_route_set(&_register_io_function, *pin_int1_route);
}

auto CoreIMU_LSM6DSOX::WriteReg(stmdev_ctx_t *ctx, uint8_t reg, const uint8_t *data, uint16_t len) const -> int32_t
{
	int32_t ret;
	ret = ctx->write_reg(ctx->handle, reg, data, len);
	return ret;
}

auto CoreIMU_LSM6DSOX::setBlockDataUpdate(stmdev_ctx_t *ctx, uint8_t val) const -> int32_t
{
	lsm6dsox_ctrl3_c_t reg;
	int32_t ret;
	ret = lsm6dsox_read_reg(ctx, LSM6DSOX_CTRL3_C, (uint8_t *)&reg, 1);

	if (ret == 0) {
		reg.bdu = val;
		ret		= lsm6dsox_write_reg(ctx, LSM6DSOX_CTRL3_C, (uint8_t *)&reg, 1);
	}

	return ret;
}

auto CoreIMU_LSM6DSOX::setIntNotification(stmdev_ctx_t *ctx, lsm6dsox_lir_t val) const -> int32_t
{
	return lsm6dsox_int_notification_set(ctx, val);
}

auto CoreIMU_LSM6DSOX::setEmbeddedSens(stmdev_ctx_t *ctx, const lsm6dsox_emb_sens_t *val) const -> int32_t
{
	lsm6dsox_emb_func_en_a_t emb_func_en_a;
	lsm6dsox_emb_func_en_b_t emb_func_en_b;
	int32_t ret;
	ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_EMBEDDED_FUNC_BANK);

	if (ret == 0) {
		ret = lsm6dsox_read_reg(ctx, LSM6DSOX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
	}

	if (ret == 0) {
		ret							 = lsm6dsox_read_reg(ctx, LSM6DSOX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
		emb_func_en_b.mlc_en		 = val->mlc;
		emb_func_en_b.fsm_en		 = val->fsm;
		emb_func_en_a.tilt_en		 = val->tilt;
		emb_func_en_a.pedo_en		 = val->step;
		emb_func_en_a.sign_motion_en = val->sig_mot;
		emb_func_en_b.fifo_compr_en	 = val->fifo_compr;
	}

	if (ret == 0) {
		ret = lsm6dsox_write_reg(ctx, LSM6DSOX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1);
	}

	if (ret == 0) {
		ret = lsm6dsox_write_reg(ctx, LSM6DSOX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
	}

	if (ret == 0) {
		ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_USER_BANK);
	}

	return ret;
}

auto CoreIMU_LSM6DSOX::getAllRessources(stmdev_ctx_t *ctx, lsm6dsox_all_sources_t *val) const -> int32_t
{
	return lsm6dsox_all_sources_get(ctx, val);
}

auto CoreIMU_LSM6DSOX::getMLCOut(stmdev_ctx_t *ctx, uint8_t *buff) const -> int32_t
{
	if (lsm6dsox_mem_bank_set(ctx, LSM6DSOX_EMBEDDED_FUNC_BANK)) return 1;

	if (lsm6dsox_read_reg(ctx, LSM6DSOX_MLC0_SRC, buff, 8)) return 1;

	if (lsm6dsox_mem_bank_set(ctx, LSM6DSOX_USER_BANK)) return 1;

	return 0;
}

}	// namespace leka
