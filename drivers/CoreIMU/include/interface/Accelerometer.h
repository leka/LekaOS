// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "lsm6dsox_reg.h"

namespace leka::interface {

using mg_t = float;

class Accelerometer
{
  public:
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

	enum class RangeXL
	{
		_2G	 = lsm6dsox_fs_xl_t::LSM6DSOX_2g,
		_4G	 = lsm6dsox_fs_xl_t::LSM6DSOX_4g,
		_8G	 = lsm6dsox_fs_xl_t::LSM6DSOX_8g,
		_16G = lsm6dsox_fs_xl_t::LSM6DSOX_16g,
	};

	struct Data {
		mg_t x;
		mg_t y;
		mg_t z;
	};

	using accel_data_t = Data;
	using odr_xl_t	   = RateXL;
	using fs_xl_t	   = RangeXL;

	virtual ~Accelerometer() = default;

	virtual auto getAccelX() -> mg_t						   = 0;
	virtual auto getAccelY() -> mg_t						   = 0;
	virtual auto getAccelZ() -> mg_t						   = 0;
	virtual auto getAccelXYZ() -> std::tuple<mg_t, mg_t, mg_t> = 0;

	virtual void setAccelRate(odr_xl_t odr_xl) = 0;
	virtual void setAccelRange(fs_xl_t fs_xl)  = 0;

	virtual auto getAccelRate() -> int32_t	= 0;
	virtual auto getAccelRange() -> int32_t = 0;

	virtual void turnOffAccel() = 0;
};
}	// namespace leka::interface
