// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_CORE_ACC_H_
#define _LEKA_OS_CORE_ACC_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"

<<<<<<< HEAD
#include "LogKit.h"
#include "interface/IMUSensor.h"

namespace leka {
    class LKCoreAccelerometer : public interface::IMUSensor
    {
        public:
            explicit LKCoreAccelerometer(LKCoreLSM6DSOX &core_imu);
            void getData(std::array<float, 3> &xl_data) final;

            void setDataRate(lsm6dsox_odr_xl_t odr_xl);
            auto getDataRate() -> int32_t final;
            
            void SetFullScale(lsm6dsox_fs_xl_t fs_xl);
            void TurnOff() final;


        private:
            leka::LKCoreLSM6DSOX &_core_imu;
=======
#include "lsm6dsox_reg.h"
#include "LogKit.h"
#include "CoreLSM6DSOX.h"
#include "interface/IMUSensor.h"

namespace leka {
    class CoreAccelerometer : public interface::IMUSensor
    {
        public:
            CoreAccelerometer(LKCoreIMU *core_imu);
            void getData(std::array<float, 3> &xl_data);

            void setDataRate(lsm6dsox_odr_xl_t odr_xl);
            auto getDataRate() -> int32_t;
            
            void SetFullScale(lsm6dsox_fs_xl_t fs_xl);
            void TurnOff();


        private:
            LKCoreIMU *_core_imu;
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
    };
}



#endif	 // _LEKA_OS_SPIKE_IMU_UTILS_H_