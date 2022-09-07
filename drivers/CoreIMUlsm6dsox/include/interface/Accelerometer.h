// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "LogKit.h"
#include <math.h>

namespace leka {
    enum odr_xl_t
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

    enum fs_xl_t
    {
        XL_FS_2g   = 0,
        XL_FS_16g  = 1,
        XL_FS_4g   = 2,
        XL_FS_8g   = 3,
    };
}

namespace leka::interface {

    class Accelerometer
    {
        public:
            
            virtual ~Accelerometer() = default;

            virtual auto getDataX_mg() -> float_t       = 0;
            virtual auto getDataY_mg() -> float_t       = 0;
            virtual auto getDataZ_mg() -> float_t       = 0;
            
            virtual auto getDataX_g() -> float_t        = 0;
            virtual auto getDataY_g() -> float_t        = 0;
            virtual auto getDataZ_g() -> float_t        = 0;

            virtual void setDataRate(odr_xl_t odr_xl)   = 0;
            virtual auto getDataRateAcc() -> int32_t    = 0;
            
            virtual void SetFullScale(fs_xl_t fs_xl)    = 0;

            virtual void TurnOffAcc()                   = 0;


        private:

    };


}
