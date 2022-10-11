// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "lsm6dsox_reg.h"

namespace leka::interface {

using dps_t = float;
class Gyroscope
{
  public:
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

	enum class RangeGyr
	{
		_125dps	 = lsm6dsox_fs_g_t::LSM6DSOX_125dps,
		_250dps	 = lsm6dsox_fs_g_t::LSM6DSOX_250dps,
		_500dps	 = lsm6dsox_fs_g_t::LSM6DSOX_500dps,
		_1000dps = lsm6dsox_fs_g_t::LSM6DSOX_1000dps,
		_2000dps = lsm6dsox_fs_g_t::LSM6DSOX_2000dps,
	};

	struct Data {
		dps_t x;
		dps_t y;
		dps_t z;
	};

	using gyro_data_t = Data;
	using odr_g_t	  = RateGyr;
	using fs_g_t	  = RangeGyr;

	virtual ~Gyroscope() = default;

	virtual auto getAngularSpeedX() -> dps_t							 = 0;
	virtual auto getAngularSpeedY() -> dps_t							 = 0;
	virtual auto getAngularSpeedZ() -> dps_t							 = 0;
	virtual auto getAngularSpeedXYZ() -> std::tuple<dps_t, dps_t, dps_t> = 0;

	virtual void setGyrRate(odr_g_t odr_gy) = 0;
	virtual void setGyrRange(fs_g_t fs_gy)	= 0;

	virtual auto getGyrRate() -> int32_t  = 0;
	virtual auto getGyrRange() -> int32_t = 0;

	virtual void turnOffGyr() = 0;
};
}	// namespace leka::interface
