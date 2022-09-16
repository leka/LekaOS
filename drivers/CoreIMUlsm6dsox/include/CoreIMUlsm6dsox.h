// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_CORE_LSM6DSOX_H_
#define _LEKA_OS_CORE_LSM6DSOX_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LogKit.h"
#include "interface/Accelerometer.h"
#include "interface/Gyroscope.h"
#include "lsm6dsox_reg.h"

using namespace std::chrono;

namespace leka {

class CoreIMULSM6DSOX : public interface::Accelerometer, public interface::Gyroscope
{
  public:
	//
	// MARK: - Enums
	//

	enum class PowerMode
	{
		Off,
		UltraLow,
		Low,
		Normal,
		High,
	};
	enum class RateXL
	{
		OFF		= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_OFF,
		_1H6	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_1Hz6,
		_12Hz5	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_12Hz5,
		_26Hz	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_26Hz,
		_52Hz	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_52Hz,
		_104Hz	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_104Hz,
		_208Hz	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_208Hz,
		_417Hz	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_417Hz,
		_833Hz	= lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_833Hz,
		_1667Hz = lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_1667Hz,
		_3333Hz = lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_3333Hz,
		_6667Hz = lsm6dsox_odr_xl_t::LSM6DSOX_XL_ODR_6667Hz,
	};

	enum class RateGyr
	{
		OFF		= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_OFF,
		_12Hz5	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_12Hz5,
		_26Hz	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_26Hz,
		_52Hz	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_52Hz,
		_104Hz	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_104Hz,
		_208Hz	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_208Hz,
		_417Hz	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_417Hz,
		_833Hz	= lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_833Hz,
		_1667Hz = lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_1667Hz,
		_3333Hz = lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_3333Hz,
		_6667Hz = lsm6dsox_odr_g_t::LSM6DSOX_GY_ODR_6667Hz,
	};

	enum class RangeXL
	{
		_2G	 = lsm6dsox_fs_xl_t::LSM6DSOX_2g,
		_4G	 = lsm6dsox_fs_xl_t::LSM6DSOX_4g,
		_8G	 = lsm6dsox_fs_xl_t::LSM6DSOX_8g,
		_16G = lsm6dsox_fs_xl_t::LSM6DSOX_16g,
	};

	enum class RangeGyr
	{
		_125dps	 = lsm6dsox_fs_g_t::LSM6DSOX_125dps,
		_250dps	 = lsm6dsox_fs_g_t::LSM6DSOX_250dps,
		_500dps	 = lsm6dsox_fs_g_t::LSM6DSOX_500dps,
		_1000dps = lsm6dsox_fs_g_t::LSM6DSOX_1000dps,
		_2000dps = lsm6dsox_fs_g_t::LSM6DSOX_2000dps,
	};

	//
	// MARK: - structs
	//

	struct SensorsData {
		accel_data_t xl;
		gyro_data_t gy;
	};

	struct ucf_line_t {
		uint8_t address;
		uint8_t data;
	};

	//
	// MARK: - Type definitions
	//

	using power_mode_t = PowerMode;
	using odr_xl_t	   = RateXL;
	using odr_g_t	   = RateGyr;
	using fs_xl_t	   = RangeXL;
	using fs_g_t	   = RangeGyr;

	//
	// MARK: - Constructor & Destructor
	//

	CoreIMULSM6DSOX(mbed::I2C &i2c, PinName pin_interrupt);
	~CoreIMULSM6DSOX() override = default;

	//
	// MARK: - Device functions
	//

	void init();
	void setPowerMode(power_mode_t);
	void setBlockDataUpdate(uint8_t val);
	void setIntNotification(lsm6dsox_lir_t val);
	void setMLCDataRate(lsm6dsox_mlc_odr_t val);
	void setEmbeddedSens(const lsm6dsox_emb_sens_t *val);

	void RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route);
	auto WriteReg(stmdev_ctx_t *ctx, uint8_t reg, const uint8_t *data, uint16_t len) const -> int32_t;

	auto getId() -> int32_t;
	auto getOutputTree(stmdev_ctx_t *ctx, uint8_t reg) -> uint8_t;
	void updateAllRessources();
	auto getConfig() -> lsm6dsox_md_t & { return _config; }
	auto getRegisterIOFunction() -> stmdev_ctx_t & { return _register_io_function; }
	auto getStatus() -> lsm6dsox_all_sources_t & { return _status; }

	void updateData();
	void TurnOffSensors();
	void TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *);

	//
	// MARK: - Accelerometer functions
	//

	auto getAccelX() -> interface::mg_t final;
	auto getAccelY() -> interface::mg_t final;
	auto getAccelZ() -> interface::mg_t final;
	auto getAccelXYZ() -> std::tuple<interface::mg_t, interface::mg_t, interface::mg_t> final;

	void setAccelRate(odr_xl_t odr_xl);
	void setAccelRange(fs_xl_t fs_xl);

	auto getAccelRate() -> int32_t final;
	auto getAccelRange() -> int32_t final;
	void turnOffAccel() final;

	//
	// MARK: - Gyroscope functions
	//
	auto getAngularSpeedX() -> interface::dps_t final;
	auto getAngularSpeedY() -> interface::dps_t final;
	auto getAngularSpeedZ() -> interface::dps_t final;

	auto getAngularSpeedXYZ() -> std::tuple<interface::dps_t, interface::dps_t, interface::dps_t> final;
	void setGyrRate(odr_g_t odr_gy);
	void setGyrRange(fs_g_t fs_gy);

	auto getGyrRate() -> int32_t final;
	auto getGyrRange() -> int32_t final;
	void turnOffGyr() final;

	auto getAccelAndAngularSpeed() -> std::tuple<interface::mg_t, interface::mg_t, interface::mg_t, interface::dps_t,
												 interface::dps_t, interface::dps_t>;
	//
	// MARK : interrupt config
	//
	template <typename T>
	void setMLCConfig(std::span<const T> tree)
	{
		// Tree writing
		for (auto line: tree) {
			WriteReg(&_register_io_function, line.address, static_cast<uint8_t *>(&line.data), 1);
			rtos::ThisThread::sleep_for(10ms);
		}

		rtos::ThisThread::sleep_for(10ms);

		TurnOffEmbeddedFeatures(&_emb_sens);
		rtos::ThisThread::sleep_for(10ms);

		TurnOffSensors();
		setBlockDataUpdate(PROPERTY_ENABLE);

		RouteSignalsInterruptGetSet(&_pin_int1_route);
		setIntNotification(LSM6DSOX_BASE_PULSED_EMB_LATCHED);
		setEmbeddedSens(&_emb_sens);

		setMLCDataRate(LSM6DSOX_ODR_PRGS_26Hz);

		setAccelRate(RateXL::_26Hz);
		setGyrRate(RateGyr::_26Hz);
		setAccelRange(RangeXL::_2G);
		setGyrRange(RangeGyr::_2000dps);
	}

  private:
	mbed::I2C &_i2c;
	uint8_t _address = LSM6DSOX_I2C_ADD_L;
	uint8_t _id;
	std::array<uint8_t, 8> _mlc_out;
	lsm6dsox_md_t _config;
	SensorsData _sensors_data;

	static const int kBufferSize			 = 32;
	std::array<uint8_t, kBufferSize> _buffer = {0};

	//
	// MARK: - For decision tree
	//

	PinName _lsm6dsox_interrupt;
	lsm6dsox_emb_sens_t _emb_sens;
	lsm6dsox_pin_int1_route_t _pin_int1_route;
	lsm6dsox_all_sources_t _status;

	//
	// MARK: - I2C Read/Write methods & pointers
	//

	stmdev_ctx_t _register_io_function;
	auto read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *p_buffer) -> int32_t;
	auto write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *p_buffer) -> int32_t;

	static auto ptr_io_write(CoreIMULSM6DSOX *handle, uint8_t write_address, uint8_t *p_buffer,
							 uint16_t number_bytes_to_write) -> int32_t;
	static auto ptr_io_read(CoreIMULSM6DSOX *handle, uint8_t read_address, uint8_t *p_buffer,
							uint16_t number_bytes_to_read) -> int32_t;
};

}	// namespace leka

#endif