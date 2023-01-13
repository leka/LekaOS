// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

enum class Position : uint8_t
{
	ear_left		 = 0,
	ear_right		 = 1,
	belt_left_back	 = 2,
	belt_left_front	 = 3,
	belt_right_back	 = 4,
	belt_right_front = 5,
};

}
