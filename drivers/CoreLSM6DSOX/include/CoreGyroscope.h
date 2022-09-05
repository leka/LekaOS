// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_CORE_GYR_H_
#define _LEKA_OS_CORE_GYR_H_

#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 91aa7087 (:construction: [drivers] test synchro)
#include "LogKit.h"
#include "interface/IMUSensor.h"

namespace leka {
    class LKCoreGyroscope : public interface::IMUSensor
    {
        public:
            explicit LKCoreGyroscope(LKCoreLSM6DSOX &core_imu) ;
            void getData(std::array<float, 3> &gy_data) final;
	        void setDataRate(lsm6dsox_odr_g_t odr_gy);
            auto getDataRate() -> int32_t final;
            void SetFullScale(lsm6dsox_fs_g_t fs_g);
            void TurnOff() final;

        private:
            leka::LKCoreLSM6DSOX &_core_imu;
<<<<<<< HEAD
=======
<<<<<<< HEAD
#include "lsm6dsox_reg.h"
#include "LogKit.h"
=======
>>>>>>> 0487f66c (:sparkles: [drivers] - Add CoreLSM6DSOX)
#include "CoreLSM6DSOX.h"
#include "interface/IMUSensor.h"

namespace leka {
    class CoreGyroscope : public interface::IMUSensor
    {
        public:
            CoreGyroscope(LKCoreIMU *core_imu);
            void getData(std::array<float, 3> &gy_data);
	        void setDataRate(lsm6dsox_odr_g_t odr_gy);
            auto getDataRate() -> int32_t;
            void SetFullScale(lsm6dsox_fs_g_t fs_g);
            void TurnOff();

        private:
            LKCoreIMU *_core_imu;
<<<<<<< HEAD
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)
>>>>>>> 0487f66c (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 98c551c9 (:construction: [drivers] CoreLSM6DSOX after sync)
    };
}



#endif	 // _LEKA_OS_SPIKE_IMU_UTILS_H_