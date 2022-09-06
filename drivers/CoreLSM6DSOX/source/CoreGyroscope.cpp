// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGyroscope.h"

namespace leka {

    LKCoreGyroscope::LKCoreGyroscope(leka::LKCoreLSM6DSOX &core_imu) : _core_imu(core_imu)
    {
        lsm6dsox_mode_get(&_core_imu.getRegisterIOFunction(), nullptr, &_core_imu.getConfig());
        
        _core_imu.getConfig().ui.gy.odr = _core_imu.getConfig().ui.gy.LSM6DSOX_GY_UI_104Hz_NM;
        _core_imu.getConfig().ui.gy.fs  = _core_imu.getConfig().ui.gy.LSM6DSOX_GY_UI_125dps;

        lsm6dsox_mode_set(&_core_imu.getRegisterIOFunction(), nullptr, &_core_imu.getConfig());

    }

    void LKCoreGyroscope::getData(std::array<float, 3> &gy_data)
	{
		lsm6dsox_data_t data;

		lsm6dsox_data_get(&_core_imu.getRegisterIOFunction(), nullptr, &_core_imu.getConfig(), &data);


		gy_data[0] = data.ui.gy.mdps[0];
		gy_data[1] = data.ui.gy.mdps[1];
		gy_data[2] = data.ui.gy.mdps[2];
	}

	// Set the ODR of the gyroscope
	// change the value of the register CTRL2_G
	void LKCoreGyroscope::setDataRate(lsm6dsox_odr_g_t odr_gy)
	{
		lsm6dsox_gy_data_rate_set(&_core_imu.getRegisterIOFunction(), odr_gy);
	}


    auto LKCoreGyroscope::getDataRate() -> int32_t
	{
		lsm6dsox_ctrl2_g_t reg;
		lsm6dsox_read_reg(&_core_imu.getRegisterIOFunction(), LSM6DSOX_CTRL2_G, (uint8_t *)&reg, 1);
	}

	void LKCoreGyroscope::SetFullScale(lsm6dsox_fs_g_t fs_g)
	{
		lsm6dsox_gy_full_scale_set(&_core_imu.getRegisterIOFunction(), fs_g);
	}

    void LKCoreGyroscope::TurnOff()
	{
		lsm6dsox_gy_data_rate_set(&_core_imu.getRegisterIOFunction(), LSM6DSOX_GY_ODR_OFF);
	}

}
