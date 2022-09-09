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

			//
			// MARK: - Enums
			//

			enum class PowerMode {
				Off,
				UltraLow,
				Low,
				Normal,
				High,
			};
			enum class OdrXL
			{
				XL_ODR_OFF    = 0,
				XL_ODR_12Hz5  = 1,
				XL_ODR_26Hz   = 2,
				XL_ODR_52Hz   = 3,
				XL_ODR_104Hz  = 4,
				XL_ODR_208Hz  = 5,
				XL_ODR_417Hz  = 6,
				XL_ODR_833Hz  = 7,
				XL_ODR_1667Hz = 8,
				XL_ODR_3333Hz = 9,
				XL_ODR_6667Hz = 10,
				XL_ODR_1Hz6   = 11,
			};
			
			enum class OdrG
			{
				GY_ODR_OFF    = 0,
				GY_ODR_12Hz5  = 1,
				GY_ODR_26Hz   = 2,
				GY_ODR_52Hz   = 3,
				GY_ODR_104Hz  = 4,
				GY_ODR_208Hz  = 5,
				GY_ODR_417Hz  = 6,
				GY_ODR_833Hz  = 7,
				GY_ODR_1667Hz = 8,
				GY_ODR_3333Hz = 9,
				GY_ODR_6667Hz = 10,
			};

			enum class FsXL
			{
				XL_FS_2g   = 0,
				XL_FS_16g  = 1,
				XL_FS_4g   = 2,
				XL_FS_8g   = 3,
			};

			enum class FsG
			{
				GY_FS_250dps   = 0,
				GY_FS_125dps   = 1,
				GY_FS_500dps   = 2,
				GY_FS_1000dps  = 4,
				GY_FS_2000dps  = 6,
			};	

			//
			// MARK: - struc
			//
            struct DataSensors {
                accel_data_t xl;
                gyro_data_t gy;
            };

			//
			// MARK: - Type definitions
			//

			using power_mode_t			= PowerMode;
			using odr_xl_t				= OdrXL;
			using odr_g_t				= OdrG;
			using fs_xl_t				= FsXL;
			using fs_g_t				= FsG;

			//
			// MARK: - Constructor & Destructor
			//

			CoreIMU_LSM6DSOX(mbed::I2C &i2c, PinName pin_interrupt);
			~CoreIMU_LSM6DSOX() final;

			//
			// MARK: - Device functions
			//

			auto init() -> int32_t;
			auto setPowerMode(power_mode_t) 							-> int32_t;
			auto setBlockDataUpdate(uint8_t val) 						-> int32_t;
			auto setIntNotification(lsm6dsox_lir_t val) 				-> int32_t;
			void setMLCDataRate(lsm6dsox_mlc_odr_t val);
			auto setEmbeddedSens(const lsm6dsox_emb_sens_t *val)		-> int32_t;

			void RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route);
			auto WriteReg(stmdev_ctx_t *ctx, uint8_t reg, const uint8_t *data, uint16_t len) const	-> int32_t;

			
			auto getId(uint8_t &id) 									-> int32_t;
			auto getMLCOut(uint8_t *buff) 								-> int32_t;
			auto getAllRessources(lsm6dsox_all_sources_t *val) 			-> int32_t;
			auto getConfig() -> lsm6dsox_md_t& { return _config; }
			auto getRegisterIOFunction() -> stmdev_ctx_t& { return _register_io_function; }

			auto updateData() 											-> int32_t;
			void TurnOffSensors();
			auto TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens) -> int32_t;

			//
			// MARK: - Accelerometer functions
			//

            auto getAccelX()                        -> interface::mg_t 	final;
            auto getAccelY()                        -> interface::mg_t 	final;
            auto getAccelZ()                        -> interface::mg_t 	final;
            auto getAccelXYZ() 						-> std::span<interface::mg_t, 3> final;

            auto setAccelRate(odr_xl_t odr_xl)		-> int32_t;
            auto setAccelRange(fs_xl_t fs_xl) 		-> int32_t;

            auto getAccelRate() 					-> int32_t 			final;
            auto getAccelRange() 					-> int32_t 			final;
            auto turnOffAccel() 					-> int32_t 			final;

			//
			// MARK: - Gyroscope functions
			//			

			auto getAngularSpeedX()                 -> interface::dps_t final;
			auto getAngularSpeedY()                 -> interface::dps_t final;
			auto getAngularSpeedZ()                 -> interface::dps_t final;
            auto getAngularSpeedXYZ() 				-> std::span<interface::dps_t, 3> final;
            auto setGyrRate(odr_g_t odr_gy) 		-> int32_t;
            auto setGyrRange(fs_g_t fs_gy) 			-> int32_t;

            auto getGyrRate() 						-> int32_t 			final;
            auto getGyrRange() 						-> int32_t 			final;
            auto turnOffGyr() 						-> int32_t 			final;
			

		private:
			mbed::I2C &_i2c;
			uint8_t _address = LSM6DSOX_I2C_ADD_L;

			PinName _lsm6dsox_interrupt;			
			lsm6dsox_md_t _config;
			DataSensors _data_sensors;

			static const int kBufferSize			 = 32;
			std::array<uint8_t, kBufferSize> _buffer = {0};

			//
			// MARK: - I2C Read/Write methods & pointers
			//

			auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer)	 			-> int32_t;
			auto write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) 			-> int32_t;

			stmdev_ctx_t _register_io_function;
			static auto ptr_io_write(CoreIMU_LSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer, 
																				uint16_t number_bytes_to_write)	-> int32_t;
			static auto ptr_io_read(CoreIMU_LSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer, 
																				uint16_t number_bytes_to_read) 	-> int32_t;

	};


}	// namespace leka

#endif