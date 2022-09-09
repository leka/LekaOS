// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "LogKit.h"
#include <math.h>


namespace leka::interface {

    using mg_t = float;

    class Accelerometer
    {
        public:
            
            struct Data {
                mg_t x;
                mg_t y;
                mg_t z;
            };
            using accel_data_t = Data;

            virtual ~Accelerometer() = default;

            virtual auto getAccelX()                            -> mg_t     = 0;
            virtual auto getAccelY()                            -> mg_t     = 0;
            virtual auto getAccelZ()                            -> mg_t     = 0;
            virtual auto getAccelXYZ() 			                -> std::span<mg_t, 3>    = 0;
            
            virtual auto getAccelRate() 				        -> int32_t  = 0;
            virtual auto getAccelRange() 				        -> int32_t  = 0;

            virtual auto turnOffAccel()                         -> int32_t  = 0;
    };
}
