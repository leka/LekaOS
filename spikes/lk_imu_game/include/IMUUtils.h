// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_IMU_UTILS_H_
#define _LEKA_OS_SPIKE_IMU_UTILS_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"

#include "lsm6dsox_reg.h"

namespace leka {
class LKCoreIMU
{
  public:
	LKCoreIMU(mbed::I2C &interface, PinName pin_interrupt);
	void getData(std::array<float, 3> &xl_data, std::array<float, 3> &gy_data);
	int32_t get_ID();	// return 0 in case of success


	static int32_t ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer, uint16_t number_bytes_to_write);
	static int32_t ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read);

	int32_t data_rate_set(lsm6dsox_odr_xl_t odr_xl, lsm6dsox_odr_g_t odr_g);
	int32_t data_rate_get(lsm6dsox_odr_xl_t *odr_xl, lsm6dsox_odr_g_t *odr_gy);
	int32_t mlc_data_rate_set(lsm6dsox_mlc_odr_t val);
	int32_t mlc_data_rate_get(lsm6dsox_mlc_odr_t *val);

	int32_t block_data_update_set(uint8_t val);
	int32_t reset_set(uint8_t val);
	int32_t reset_get(uint8_t *val);
	int32_t int_notification_set(lsm6dsox_lir_t val);
	int32_t embedded_sens_set(lsm6dsox_emb_sens_t *val);
	int32_t rounding_mode_set(lsm6dsox_rounding_t val);
	int32_t all_sources_get(lsm6dsox_all_sources_t *val);
	int32_t mlc_out_get(uint8_t *buff);


	void TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens);
	void TurnOffSensors();
	void SetFullScale();
	void RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route);

	stmdev_ctx_t& get_reg_function(){
		return _register_io_function;
	}

	//Hard_fault if we use ptr_io_write()
	int32_t write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer);


  private:
	int32_t read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer);
	//int32_t write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer);

	mbed::I2C &_interface;
	uint8_t _address = LSM6DSOX_I2C_ADD_L;
	PinName _lsm6dsox_interrupt;

	stmdev_ctx_t _register_io_function;
	lsm6dsox_md_t _config;
};
}	// namespace leka

void runIMU(leka::LKCoreIMU *imu);

#endif	 // _LEKA_OS_SPIKE_IMU_UTILS_H_
