// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0


#ifndef _LEKA_OS_CORE_LSM6DSOX_H_
#define _LEKA_OS_CORE_LSM6DSOX_H_


#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"

#include "lsm6dsox_reg.h"
#include "LogKit.h"
#include "interface/Accelerometer.h"
#include "interface/Gyroscope.h"

namespace leka {

	class CoreIMU_LSM6DSOX : public interface::Accelerometer, public interface::Gyroscope
	{
		public:
			CoreIMU_LSM6DSOX(mbed::I2C &interface, PinName pin_interrupt);

			auto getId() -> int32_t;

			void TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens);
			void TurnOffSensors();
			void RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route);
			void print_uint8_t(uint8_t n);

			auto getAllRessources(stmdev_ctx_t *ctx, lsm6dsox_all_sources_t *val) const -> int32_t;

			auto WriteReg(stmdev_ctx_t *ctx, uint8_t reg, const uint8_t *data, uint16_t len) const	-> int32_t;
			auto setBlockDataUpdate(stmdev_ctx_t *ctx, uint8_t val) const							-> int32_t;
			auto setIntNotification(stmdev_ctx_t *ctx, lsm6dsox_lir_t val) const					-> int32_t;
			auto setEmbeddedSens(stmdev_ctx_t *ctx, const lsm6dsox_emb_sens_t *val)	const			-> int32_t;
			auto setMLCDataRate(stmdev_ctx_t *ctx, lsm6dsox_mlc_odr_t val) const					-> int32_t;
			auto getMLCOut(stmdev_ctx_t *ctx, uint8_t *buff) const									-> int32_t;

			static auto ptr_io_write(CoreIMU_LSM6DSOX *handle, uint8_t write_address, const uint8_t *p_buffer, uint16_t number_bytes_to_write) -> int32_t;
			static auto ptr_io_read(CoreIMU_LSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read) -> int32_t;

			auto getRegisterIOFunction() -> stmdev_ctx_t& { return _register_io_function; }
			auto getConfig() -> lsm6dsox_md_t& { return _config; }


			//Accelerometer
			auto getDataX_mg() -> float_t final;
            auto getDataY_mg() -> float_t final;
            auto getDataZ_mg() -> float_t final;
            auto getDataX_g() -> float_t final;
            auto getDataY_g() -> float_t final;
            auto getDataZ_g() -> float_t final;
            void setDataRate(odr_xl_t odr_xl) final;
            auto getDataRateAcc() -> int32_t final;
            void SetFullScale(fs_xl_t fs_xl) final;
            void TurnOffAcc() final;

			//Gyroscope
			auto getDataX_mdps() -> float_t final;
            auto getDataY_mdps() -> float_t final;
            auto getDataZ_mdps() -> float_t final;
            auto getDataX_dps() -> float_t final;
            auto getDataY_dps() -> float_t final;
            auto getDataZ_dps() -> float_t final;
            void setDataRate(odr_g_t odr_gy) final;
            auto getDataRateGyr() -> int32_t final;
            void SetFullScale(fs_g_t fs_gy) final;
            void TurnOffGyr() final;


		private:
			auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t;
			auto write(uint8_t register_address, uint16_t number_bytes_to_write, const uint8_t *p_buffer) -> int32_t;

			mbed::I2C &_interface;
			uint8_t _address = LSM6DSOX_I2C_ADD_L;
			PinName _lsm6dsox_interrupt;

			stmdev_ctx_t _register_io_function;
			lsm6dsox_md_t _config;
	};


}	// namespace leka

#endif