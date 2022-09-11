// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "LogKit.h"



namespace leka::interface {

    using dps_t = float;
    class Gyroscope
    {
        public:
            
            struct Data {
                dps_t x;
                dps_t y;
                dps_t z;
            };
            using gyro_data_t = Data;
            
            virtual ~Gyroscope() = default;

            virtual auto getAngularSpeedX()                                 -> dps_t    = 0;
            virtual auto getAngularSpeedY()                                 -> dps_t    = 0;
            virtual auto getAngularSpeedZ()                                 -> dps_t    = 0;
            virtual auto getAngularSpeedXYZ() 						        -> std::tuple<dps_t, dps_t, dps_t> = 0;

            virtual auto getGyrRate() 						                -> int32_t  = 0;
            virtual auto getGyrRange() 						                -> int32_t  = 0;

            virtual void turnOffGyr()                                                   = 0;
    };
}
