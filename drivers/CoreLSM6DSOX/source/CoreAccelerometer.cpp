// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAccelerometer.h"

namespace leka {

    LKCoreAccelerometer::LKCoreAccelerometer(leka::LKCoreLSM6DSOX &core_imu) : _core_imu(core_imu)
    {
        lsm6dsox_mode_get(&_core_imu.getRegisterIOFunction(), nullptr, &_core_imu.getConfig());
        
        _core_imu.getConfig().ui.xl.odr = _core_imu.getConfig().ui.xl.LSM6DSOX_XL_UI_104Hz_NM;
        _core_imu.getConfig().ui.xl.fs  = _core_imu.getConfig().ui.xl.LSM6DSOX_XL_UI_2g;

        lsm6dsox_mode_set(&_core_imu.getRegisterIOFunction(), nullptr, &_core_imu.getConfig());
    }

    void LKCoreAccelerometer::getData(std::array<float, 3> &xl_data)
	{
		lsm6dsox_data_t data;

		lsm6dsox_data_get(&_core_imu.getRegisterIOFunction(), nullptr, &_core_imu.getConfig(), &data);

		xl_data[0] = data.ui.xl.mg[0];
		xl_data[1] = data.ui.xl.mg[1];
		xl_data[2] = data.ui.xl.mg[2];
	}
    
	// Set the ODR of the accelerometer
	// change the value of the register CTRL1_XL
    void LKCoreAccelerometer::setDataRate(lsm6dsox_odr_xl_t odr_xl)
	{
		lsm6dsox_xl_data_rate_set(&_core_imu.getRegisterIOFunction(), odr_xl);
	}

	auto LKCoreAccelerometer::getDataRate() -> int32_t
	{
		lsm6dsox_ctrl1_xl_t reg;
		lsm6dsox_read_reg(&_core_imu.getRegisterIOFunction(), LSM6DSOX_CTRL1_XL, (uint8_t *)&reg, 1);
        
        return reg.odr_xl;
	}
    
	void LKCoreAccelerometer::SetFullScale(lsm6dsox_fs_xl_t fs_xl)
	{
		lsm6dsox_xl_full_scale_set(&_core_imu.getRegisterIOFunction(), fs_xl);
	}
    
    void LKCoreAccelerometer::TurnOff()
	{
		lsm6dsox_xl_data_rate_set(&_core_imu.getRegisterIOFunction(), LSM6DSOX_XL_ODR_OFF);
	}

}
