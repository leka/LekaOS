// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_CORE_GYR_H_
#define _LEKA_OS_CORE_GYR_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"

#include "LogKit.h"
#include "interface/IMUSensor.h"

namespace leka {
    class LKCoreGyroscope : public interface::IMUSensor
    {
        public:
            explicit LKCoreGyroscope(CoreIMU_LSM6DSOX &core_imu) ;
            void getData(std::array<float, 3> &gy_data) final;
	        void setDataRate(lsm6dsox_odr_g_t odr_gy);
            auto getDataRate() -> int32_t final;
            void SetFullScale(lsm6dsox_fs_g_t fs_g);
            void TurnOff() final;

        private:
            leka::CoreIMU_LSM6DSOX &_core_imu;

    };
}



#endif	 // _LEKA_OS_SPIKE_IMU_UTILS_H_