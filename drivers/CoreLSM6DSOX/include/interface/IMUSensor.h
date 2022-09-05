// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "lsm6dsox_reg.h"
#include "CoreLSM6DSOX.h"
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)
>>>>>>> 0487f66c (:sparkles: [drivers] - Add CoreLSM6DSOX)
=======
>>>>>>> 98c551c9 (:construction: [drivers] CoreLSM6DSOX after sync)

namespace leka::interface {

    class IMUSensor
    {
<<<<<<< HEAD
        public:
            virtual ~IMUSensor() = default;

            virtual void getData(std::array<float, 3> &data_array) = 0;
            virtual auto getDataRate() -> int32_t = 0;
            virtual void TurnOff() = 0;
=======
    public:
        virtual ~IMUSensor() = default;

        virtual void getData(std::array<float, 3> &data_array) = 0;
        virtual auto getDataRate() -> int32_t =0;
        virtual void TurnOff() = 0;
>>>>>>> 22302a25 (:sparkles: [drivers] - Add CoreLSM6DSOX)

    };

}	// namespace leka::interface
