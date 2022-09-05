// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLSM6DSOX.h"

namespace leka {

	LKCoreLSM6DSOX::LKCoreLSM6DSOX(mbed::I2C &interface, PinName pin_interrupt)
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


	auto LKCoreLSM6DSOX::ptr_io_write(LKCoreLSM6DSOX *handle, uint8_t write_address, const uint8_t *p_buffer, uint16_t number_bytes_to_write) -> int32_t
	{
		return handle->write(write_address, number_bytes_to_write, p_buffer);
	}

	auto LKCoreLSM6DSOX::ptr_io_read(LKCoreLSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read) -> int32_t
	{
		return handle->read(read_address, number_bytes_to_read, p_buffer);
	}

	auto LKCoreLSM6DSOX::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *pBuffer) -> int32_t
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

	auto LKCoreLSM6DSOX::write(uint8_t register_address, uint16_t number_bytes_to_write, const uint8_t *pBuffer) -> int32_t
	{
		int ret;
		uint8_t tmp[32];

		if (number_bytes_to_write >= 32) return 2;

		/* First, send component address. Then, send data and STOP condition */
		tmp[0] = register_address;
		memcpy(tmp + 1, pBuffer, number_bytes_to_write);

		ret = _interface.write(_address, (const char *)tmp, number_bytes_to_write + 1, false);

		if (ret) return 1;
		return 0;
	}

	// Return 0 in case of success
	auto LKCoreLSM6DSOX::getId() -> int32_t
	{
		lsm6dsox_id_t val;
		if (lsm6dsox_id_get(&_register_io_function, nullptr, &val) != 0)
			return 1;
		else
			return 0;
	}


	auto LKCoreLSM6DSOX::setMLCDataRate(stmdev_ctx_t *ctx, lsm6dsox_mlc_odr_t val) const -> int32_t
	{
		lsm6dsox_emb_func_odr_cfg_c_t reg;
		int32_t ret;
		ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_EMBEDDED_FUNC_BANK);

		if (ret == 0) {
			ret = lsm6dsox_read_reg(ctx, LSM6DSOX_EMB_FUNC_ODR_CFG_C, (uint8_t *)&reg, 1);
		}

		if (ret == 0) {
			reg.mlc_odr = (uint8_t)val;
			ret			= lsm6dsox_write_reg(ctx, LSM6DSOX_EMB_FUNC_ODR_CFG_C, (uint8_t *)&reg, 1);
		}

		if (ret == 0) {
			ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_USER_BANK);
		}

		return ret;
	}

	void LKCoreLSM6DSOX::TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens)
	{
		lsm6dsox_embedded_sens_get(&_register_io_function, emb_sens);
		lsm6dsox_embedded_sens_off(&_register_io_function);
	}

	void LKCoreLSM6DSOX::TurnOffSensors()
	{
		lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF);
		lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF);
	}

	void LKCoreLSM6DSOX::RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route)
	{
		lsm6dsox_pin_int1_route_get(&_register_io_function, pin_int1_route);
		pin_int1_route->mlc1 = PROPERTY_ENABLE;
		lsm6dsox_pin_int1_route_set(&_register_io_function, *pin_int1_route);
	}

	auto LKCoreLSM6DSOX::WriteReg(stmdev_ctx_t *ctx, uint8_t reg, const uint8_t *data, uint16_t len) const -> int32_t
	{
		int32_t ret;
		ret = ctx->write_reg(ctx->handle, reg, data, len);
		return ret;
	}

	auto LKCoreLSM6DSOX::setBlockDataUpdate(stmdev_ctx_t *ctx, uint8_t val) const -> int32_t
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

	auto LKCoreLSM6DSOX::setIntNotification(stmdev_ctx_t *ctx, lsm6dsox_lir_t val) const -> int32_t
	{
		lsm6dsox_tap_cfg0_t tap_cfg0;
		lsm6dsox_page_rw_t page_rw;
		int32_t ret;
		ret = lsm6dsox_read_reg(ctx, LSM6DSOX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);

		if (ret == 0) {
			tap_cfg0.lir			 = static_cast<uint8_t>(val) & 0x01U;
			tap_cfg0.int_clr_on_read = static_cast<uint8_t>(val) & 0x01U;
			ret						 = lsm6dsox_write_reg(ctx, LSM6DSOX_TAP_CFG0, (uint8_t *)&tap_cfg0, 1);
		}

		if (ret == 0) {
			ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_EMBEDDED_FUNC_BANK);
		}

		if (ret == 0) {
			ret = lsm6dsox_read_reg(ctx, LSM6DSOX_PAGE_RW, (uint8_t *)&page_rw, 1);
		}

		if (ret == 0) {
			page_rw.emb_func_lir = ((uint8_t)val & 0x02U) >> 1;
			ret					 = lsm6dsox_write_reg(ctx, LSM6DSOX_PAGE_RW, (uint8_t *)&page_rw, 1);
		}

		if (ret == 0) {
			ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_USER_BANK);
		}

		return ret;
	}

	auto LKCoreLSM6DSOX::setEmbeddedSens(stmdev_ctx_t *ctx, const lsm6dsox_emb_sens_t *val) const -> int32_t
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

	auto LKCoreLSM6DSOX::getAllRessources(stmdev_ctx_t *ctx, lsm6dsox_all_sources_t *val) -> int32_t
	{
		lsm6dsox_emb_func_status_mainpage_t emb_func_status_mainpage;
		lsm6dsox_status_master_mainpage_t status_master_mainpage;
		lsm6dsox_fsm_status_a_mainpage_t fsm_status_a_mainpage;
		lsm6dsox_fsm_status_b_mainpage_t fsm_status_b_mainpage;
		lsm6dsox_mlc_status_mainpage_t mlc_status_mainpage;
		lsm6dsox_fifo_status1_t fifo_status1;
		lsm6dsox_fifo_status2_t fifo_status2;
		lsm6dsox_all_int_src_t all_int_src;
		lsm6dsox_wake_up_src_t wake_up_src;
		lsm6dsox_status_reg_t status_reg;
		lsm6dsox_tap_src_t tap_src;
		lsm6dsox_d6d_src_t d6d_src;
		lsm6dsox_ctrl5_c_t ctrl5_c;
		uint8_t reg[12];

		if(lsm6dsox_read_reg(ctx, LSM6DSOX_CTRL5_C, (uint8_t*)&ctrl5_c, 1)) //If 0, then no error
			return 1; //Error

		ctrl5_c.rounding_status = PROPERTY_ENABLE;

		if(lsm6dsox_write_reg(ctx, LSM6DSOX_CTRL5_C, (uint8_t*)&ctrl5_c, 1))
			return 1;

		if(lsm6dsox_read_reg(ctx, LSM6DSOX_ALL_INT_SRC, reg, 12))
			return 1;

		byteCopy((uint8_t*)&all_int_src, &reg[0]);
		byteCopy((uint8_t*)&wake_up_src, &reg[1]);
		byteCopy((uint8_t*)&tap_src, &reg[2]);
		byteCopy((uint8_t*)&d6d_src, &reg[3]);
		byteCopy((uint8_t*)&status_reg, &reg[4]);
		byteCopy((uint8_t*)&emb_func_status_mainpage, &reg[5]);
		byteCopy((uint8_t*)&fsm_status_a_mainpage, &reg[6]);
		byteCopy((uint8_t*)&fsm_status_b_mainpage, &reg[7]);
		byteCopy((uint8_t*)&mlc_status_mainpage, &reg[8]);
		byteCopy((uint8_t*)&status_master_mainpage, &reg[9]);
		byteCopy((uint8_t*)&fifo_status1, &reg[10]);
		byteCopy((uint8_t*)&fifo_status2, &reg[11]);
		if(val == nullptr)
			return 1;
		val->timestamp			= all_int_src.timestamp_endcount;
		val->wake_up_z			= wake_up_src.z_wu;
		val->wake_up_y			= wake_up_src.y_wu;
		val->wake_up_x			= wake_up_src.x_wu;
		val->wake_up			= wake_up_src.wu_ia;
		val->sleep_state		= wake_up_src.sleep_state;
		val->free_fall			= wake_up_src.ff_ia;
		val->sleep_change		= wake_up_src.sleep_change_ia;
		val->tap_x				= tap_src.x_tap;
		val->tap_y				= tap_src.y_tap;
		val->tap_z				= tap_src.z_tap;
		val->tap_sign			= tap_src.tap_sign;
		val->double_tap			= tap_src.double_tap;
		val->single_tap			= tap_src.single_tap;
		val->six_d_xl			= d6d_src.xl;
		val->six_d_xh			= d6d_src.xh;
		val->six_d_yl			= d6d_src.yl;
		val->six_d_yh			= d6d_src.yh;
		val->six_d_zl			= d6d_src.zl;
		val->six_d_zh			= d6d_src.zh;
		val->six_d				= d6d_src.d6d_ia;
		val->den_flag			= d6d_src.den_drdy;
		val->drdy_xl			= status_reg.xlda;
		val->drdy_g				= status_reg.gda;
		val->drdy_temp			= status_reg.tda;
		val->step_detector		= emb_func_status_mainpage.is_step_det;
		val->tilt				= emb_func_status_mainpage.is_tilt;
		val->sig_mot			= emb_func_status_mainpage.is_sigmot;
		val->fsm_lc				= emb_func_status_mainpage.is_fsm_lc;
		val->fsm1				= fsm_status_a_mainpage.is_fsm1;
		val->fsm2				= fsm_status_a_mainpage.is_fsm2;
		val->fsm3				= fsm_status_a_mainpage.is_fsm3;
		val->fsm4				= fsm_status_a_mainpage.is_fsm4;
		val->fsm5				= fsm_status_a_mainpage.is_fsm5;
		val->fsm6				= fsm_status_a_mainpage.is_fsm6;
		val->fsm7				= fsm_status_a_mainpage.is_fsm7;
		val->fsm8				= fsm_status_a_mainpage.is_fsm8;
		val->fsm9				= fsm_status_b_mainpage.is_fsm9;
		val->fsm10				= fsm_status_b_mainpage.is_fsm10;
		val->fsm11				= fsm_status_b_mainpage.is_fsm11;
		val->fsm12				= fsm_status_b_mainpage.is_fsm12;
		val->fsm13				= fsm_status_b_mainpage.is_fsm13;
		val->fsm14				= fsm_status_b_mainpage.is_fsm14;
		val->fsm15				= fsm_status_b_mainpage.is_fsm15;
		val->fsm16				= fsm_status_b_mainpage.is_fsm16;
		val->mlc1				= mlc_status_mainpage.is_mlc1;
		val->mlc2				= mlc_status_mainpage.is_mlc2;
		val->mlc3				= mlc_status_mainpage.is_mlc3;
		val->mlc4				= mlc_status_mainpage.is_mlc4;
		val->mlc5				= mlc_status_mainpage.is_mlc5;
		val->mlc6				= mlc_status_mainpage.is_mlc6;
		val->mlc7				= mlc_status_mainpage.is_mlc7;
		val->mlc8				= mlc_status_mainpage.is_mlc8;
		val->sh_endop			= status_master_mainpage.sens_hub_endop;
		val->sh_slave0_nack		= status_master_mainpage.slave0_nack;
		val->sh_slave1_nack		= status_master_mainpage.slave1_nack;
		val->sh_slave2_nack		= status_master_mainpage.slave2_nack;
		val->sh_slave3_nack		= status_master_mainpage.slave3_nack;
		val->sh_wr_once			= status_master_mainpage.wr_once_done;
		val->fifo_diff			= (256U * fifo_status2.diff_fifo) + fifo_status1.diff_fifo;
		val->fifo_ovr_latched	= fifo_status2.over_run_latched;
		val->fifo_bdr			= fifo_status2.counter_bdr_ia;
		val->fifo_full			= fifo_status2.fifo_full_ia;
		val->fifo_ovr			= fifo_status2.fifo_ovr_ia;
		val->fifo_th			= fifo_status2.fifo_wtm_ia;
		ctrl5_c.rounding_status = PROPERTY_DISABLE;
		if(lsm6dsox_write_reg(ctx, LSM6DSOX_CTRL5_C, (uint8_t*)&ctrl5_c, 1))
			return 1;

		return 0; //No error
	}

	auto LKCoreLSM6DSOX::getMLCOut(stmdev_ctx_t *ctx, uint8_t *buff) -> int32_t
	{
		int32_t ret;
		ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_EMBEDDED_FUNC_BANK);

		if (ret == 0) {
			ret = lsm6dsox_read_reg(ctx, LSM6DSOX_MLC0_SRC, buff, 8);
		}

		if (ret == 0) {
			ret = lsm6dsox_mem_bank_set(ctx, LSM6DSOX_USER_BANK);
		}

		return ret;
	}

	static void byteCopy(uint8_t *target, const uint8_t *source)
	{
		if ((target != nullptr) && (source != nullptr)) 
			*target = *source;
	}

}	// namespace leka




