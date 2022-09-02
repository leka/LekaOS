// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "lsm6dsox_reg.h"
<<<<<<< HEAD
#include "CoreLSM6DSOX.h"
=======
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)

namespace leka::interface {

    class IMUSensor
    {
    public:
        virtual ~IMUSensor() = default;

        virtual void getData(std::array<float, 3> &data_array) = 0;
        virtual auto getDataRate() -> int32_t =0;
        virtual void TurnOff() = 0;

    };

}	// namespace leka::interface
