// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIMUlsm6dsox.h"

namespace leka {

//
// MARK: - Constructor
//

CoreIMU_LSM6DSOX::CoreIMU_LSM6DSOX(mbed::I2C &i2c, PinName pin_interrupt)
	: _i2c(i2c), _lsm6dsox_interrupt(pin_interrupt)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;
}

//
// Mark: - Device functions
//

auto CoreIMU_LSM6DSOX::init() -> int32_t
{
	if(lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY)) {
		return 1;
	}

	if(lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE)) {
		return 1;
	}

	if(setPowerMode(PowerMode::Normal)) {
		return 1;
	}
	if(lsm6dsox_mode_get(&_register_io_function, nullptr, &_config)) {
		return 1;
	}

	_config.ui.xl.odr = _config.ui.xl.LSM6DSOX_XL_UI_104Hz_NM;
	_config.ui.xl.fs  = _config.ui.xl.LSM6DSOX_XL_UI_2g;
	_config.ui.gy.odr = _config.ui.gy.LSM6DSOX_GY_UI_104Hz_NM;
	_config.ui.gy.fs  = _config.ui.gy.LSM6DSOX_GY_UI_125dps;

	if(lsm6dsox_mode_set(&_register_io_function, nullptr, &_config)) {
		return 1;
	}
	
	if(setAccelRate(RateXL::_26Hz)) {
		return 1;
	}
	if(setGyrRate(RateGyr::_26Hz)) {
		return 1;
	}
	if(setAccelRange(RangeXL::_2G)) {
		return 1;
	}
	if(setGyrRange(RangeGyr::_2000dps)) {
		return 1;
	}

	return 0;
}

auto CoreIMU_LSM6DSOX::WriteReg(stmdev_ctx_t *ctx, uint8_t reg, const uint8_t *data, uint16_t len) const -> int32_t
{
	auto ret = ctx->write_reg(ctx->handle, reg, data, len);
	return ret;
}

//
// MARK: - I2C Read/Write methods & pointers
//

auto CoreIMU_LSM6DSOX::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *pBuffer) -> int32_t
{
		/* Send component address, with no STOP condition */
	auto ret = _i2c.write(_address, (const char *)&register_address, 1, true);
	if (ret == 0) {
		/* Read data, with STOP condition  */
		ret = _i2c.read(_address, (char *)pBuffer, number_bytes_to_read, false);
	}

	return ret;
}

auto CoreIMU_LSM6DSOX::write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *pBuffer) -> int32_t
{
	if (number_bytes_to_write >= 32) return 2;

	/* First, send component address. Then, send data and STOP condition */
	pBuffer[0] = register_address;
	std::copy(pBuffer, (pBuffer + number_bytes_to_write), (_buffer.begin() + 1));
	auto ret = _i2c.write(_address, (const char *)_buffer.data(), number_bytes_to_write + 1, false);

	return ret;
}


auto CoreIMU_LSM6DSOX::ptr_io_write(CoreIMU_LSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer, 
									uint16_t number_bytes_to_write) -> int32_t
{
	return handle->write(write_address, number_bytes_to_write, p_buffer);
}	


auto CoreIMU_LSM6DSOX::ptr_io_read(CoreIMU_LSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
								   uint16_t number_bytes_to_read) -> int32_t
{
	return handle->read(read_address, number_bytes_to_read, p_buffer);
}

//
// Mark: - Device functions
//

auto CoreIMU_LSM6DSOX::setPowerMode(power_mode_t mode) -> int32_t
{
	if (mode == PowerMode::Off) {
		if (lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF)) {
			return 1;
		}
	} else {
		lsm6dsox_xl_hm_mode_t power_mode;
		switch (mode) {
			case PowerMode::UltraLow:
				power_mode = LSM6DSOX_ULTRA_LOW_POWER_MD;
				break;
			case PowerMode::Low:
				power_mode = LSM6DSOX_LOW_NORMAL_POWER_MD;
				break;
			case PowerMode::Normal:
				power_mode = LSM6DSOX_LOW_NORMAL_POWER_MD;
				break;
			case PowerMode::High:
				power_mode = LSM6DSOX_HIGH_PERFORMANCE_MD;
				break;
			default:
				return 1;
		}

		if (lsm6dsox_xl_power_mode_set(&getRegisterIOFunction(), power_mode)) {
			return 1;
		}
	}
	return 0;
}

void CoreIMU_LSM6DSOX::setMLCDataRate(lsm6dsox_mlc_odr_t val)
{
	lsm6dsox_mlc_data_rate_set(&getRegisterIOFunction(), val);
}

auto CoreIMU_LSM6DSOX::TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens) -> int32_t
{
	if(lsm6dsox_embedded_sens_get(&_register_io_function, emb_sens)){
		return 1;
	}
	if(lsm6dsox_embedded_sens_off(&_register_io_function)){
		return 1;
	}
	return 0;
}

void CoreIMU_LSM6DSOX::RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route)
{
	lsm6dsox_pin_int1_route_get(&_register_io_function, pin_int1_route);
	pin_int1_route->mlc1 = PROPERTY_ENABLE;
	lsm6dsox_pin_int1_route_set(&_register_io_function, *pin_int1_route);
}

auto CoreIMU_LSM6DSOX::setBlockDataUpdate(uint8_t val)  -> int32_t
{
	lsm6dsox_ctrl3_c_t reg;
	auto ret = lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_CTRL3_C, (uint8_t *)&reg, 1);

	if (ret == 0) {
		reg.bdu = val;
		ret		= lsm6dsox_write_reg(&getRegisterIOFunction(), LSM6DSOX_CTRL3_C, (uint8_t *)&reg, 1);
	}
	return ret;
}

auto CoreIMU_LSM6DSOX::setIntNotification(lsm6dsox_lir_t val) -> int32_t
{
	return lsm6dsox_int_notification_set(&getRegisterIOFunction(), val);
}

auto CoreIMU_LSM6DSOX::setEmbeddedSens(const lsm6dsox_emb_sens_t *val) -> int32_t
{
	lsm6dsox_emb_func_en_a_t emb_func_en_a;
	lsm6dsox_emb_func_en_b_t emb_func_en_b;
	if(lsm6dsox_mem_bank_set(&getRegisterIOFunction(), LSM6DSOX_EMBEDDED_FUNC_BANK)) {
		return 1;
	}

	if (lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1)) {
		return 1;
	}

	auto ret					 = lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1);
	emb_func_en_b.mlc_en		 = val->mlc;
	emb_func_en_b.fsm_en		 = val->fsm;
	emb_func_en_a.tilt_en		 = val->tilt;
	emb_func_en_a.pedo_en		 = val->step;
	emb_func_en_a.sign_motion_en = val->sig_mot;
	emb_func_en_b.fifo_compr_en	 = val->fifo_compr;

	if (ret == 0) {
		if(lsm6dsox_write_reg(&getRegisterIOFunction(), LSM6DSOX_EMB_FUNC_EN_A, (uint8_t *)&emb_func_en_a, 1)) {
			return 1;
		}
		if(lsm6dsox_write_reg(&getRegisterIOFunction(), LSM6DSOX_EMB_FUNC_EN_B, (uint8_t *)&emb_func_en_b, 1)) {
			return 1;
		}
		if(lsm6dsox_mem_bank_set(&getRegisterIOFunction(), LSM6DSOX_USER_BANK)) {
			return 1;
		}
	}

	return 0;
}

auto CoreIMU_LSM6DSOX::getAllRessources(lsm6dsox_all_sources_t *val) -> int32_t
{
	if(lsm6dsox_all_sources_get(&getRegisterIOFunction(), val)) {
		return 1;
	}
	return 0;
}

auto CoreIMU_LSM6DSOX::getId(uint8_t &id) -> int32_t
{
	if (lsm6dsox_device_id_get(&_register_io_function, &id)) {
		return 0; 
	}
	return id;
}
	//A revoir
auto CoreIMU_LSM6DSOX::getMLCOut(uint8_t *buff) -> int32_t
{
	if (lsm6dsox_mem_bank_set(&getRegisterIOFunction(), LSM6DSOX_EMBEDDED_FUNC_BANK)) {
		return 1; 
	} 
	if (lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_MLC0_SRC, buff, 8)) {
		return 1; 
	}
	if (lsm6dsox_mem_bank_set(&getRegisterIOFunction(), LSM6DSOX_USER_BANK)) {
		return 1; 
	}

	return 0;
}


auto CoreIMU_LSM6DSOX::updateData() -> int32_t
{
    lsm6dsox_data_t data;
    if(lsm6dsox_data_get(&getRegisterIOFunction(), nullptr, &getConfig(), &data)) {
		return 1;
	}
    _data_sensors.xl.x = data.ui.xl.mg[0];
    _data_sensors.xl.y = data.ui.xl.mg[1];
    _data_sensors.xl.z = data.ui.xl.mg[2];
	_data_sensors.gy.x = data.ui.gy.mdps[0]/1000;
	_data_sensors.gy.y = data.ui.gy.mdps[1]/1000;
	_data_sensors.gy.z = data.ui.gy.mdps[2]/1000;

	return 0;
}

void CoreIMU_LSM6DSOX::TurnOffSensors()
{
	turnOffAccel();
	turnOffGyr();
}	

//
// Mark: - Acceleremeter functions
//

auto CoreIMU_LSM6DSOX::setAccelRate(odr_xl_t odr_xl) -> int32_t
{
	if(lsm6dsox_xl_data_rate_set(&getRegisterIOFunction(), static_cast<lsm6dsox_odr_xl_t>(odr_xl))) {
		return 1;
	}
	return 0;
}

auto CoreIMU_LSM6DSOX::setAccelRange(fs_xl_t fs_xl) -> int32_t
{
	if(lsm6dsox_xl_full_scale_set(&getRegisterIOFunction(), static_cast<lsm6dsox_fs_xl_t>(fs_xl))) {
		return 1;
	}
	return 0;
}

auto CoreIMU_LSM6DSOX::getAccelRate() -> int32_t
{
	lsm6dsox_ctrl1_xl_t reg;
	lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_CTRL1_XL, (uint8_t *)&reg, 1);
	return reg.odr_xl;
}

auto CoreIMU_LSM6DSOX::getAccelRange() -> int32_t
{
	lsm6dsox_ctrl1_xl_t reg;
	lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_CTRL1_XL, (uint8_t *)&reg, 1);
	return reg.fs_xl;
}

auto CoreIMU_LSM6DSOX::getAccelX() -> interface::mg_t 
{
    return _data_sensors.xl.x;
}

auto CoreIMU_LSM6DSOX::getAccelY() -> interface::mg_t
{
    return _data_sensors.xl.y;
}            

auto CoreIMU_LSM6DSOX::getAccelZ() -> interface::mg_t 
{
    return _data_sensors.xl.z;
}

auto CoreIMU_LSM6DSOX::getAccelXYZ() -> std::span<interface::mg_t, 3>
{
	auto xl_data = std::to_array({getAccelX(), getAccelY(), getAccelZ()});
	return std::span {xl_data};
}


auto CoreIMU_LSM6DSOX::turnOffAccel() -> int32_t 
{
	if(lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF)) {
		return 1;
	}
	return 0;
}


//
// Mark: - Gyroscope functions
//

auto CoreIMU_LSM6DSOX::setGyrRate(odr_g_t odr_gy) -> int32_t
{
	if(lsm6dsox_gy_data_rate_set(&getRegisterIOFunction(), static_cast<lsm6dsox_odr_g_t>(odr_gy))) {
		return 1;
	}
	return 0;
}

auto CoreIMU_LSM6DSOX::setGyrRange(fs_g_t fs_gy) -> int32_t
{
	if(lsm6dsox_gy_full_scale_set(&getRegisterIOFunction(), static_cast<lsm6dsox_fs_g_t>(fs_gy))) {
		return 1;
	}
	return 0;
}

auto CoreIMU_LSM6DSOX::getGyrRate() -> int32_t
{
	lsm6dsox_ctrl2_g_t reg;
	lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_CTRL2_G, (uint8_t *)&reg, 1);
	return reg.odr_g;
}

auto CoreIMU_LSM6DSOX::getGyrRange() -> int32_t
{
	lsm6dsox_ctrl2_g_t reg;
	lsm6dsox_read_reg(&getRegisterIOFunction(), LSM6DSOX_CTRL2_G, (uint8_t *)&reg, 1);
	return reg.fs_g;
}

auto CoreIMU_LSM6DSOX::getAngularSpeedX() -> interface::dps_t
{
	return _data_sensors.gy.x;
}

auto CoreIMU_LSM6DSOX::getAngularSpeedY() -> interface::dps_t
{
	return _data_sensors.gy.y;
}     

auto CoreIMU_LSM6DSOX::getAngularSpeedZ() -> interface::dps_t
{
	return _data_sensors.gy.z;
}     

auto CoreIMU_LSM6DSOX::getAngularSpeedXYZ()  -> std::span<interface::dps_t, 3>
{
	auto gy_data = std::to_array({getAngularSpeedX(), getAngularSpeedY(), getAngularSpeedZ()});
	return std::span {gy_data};
}

auto CoreIMU_LSM6DSOX::turnOffGyr() -> int32_t 
{
	if(	lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF)) {
		return 1;
	}
	return  0;
}

}	// namespace leka
