// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGyroscope.h"

namespace leka {

auto CoreGyroscope::getXYZ() -> std::tuple<dps_t, dps_t, dps_t>
{
	auto data = _lsm6dsox.getData();
	return std::tuple {data.gy.x, data.gy.y, data.gy.z};
}

}	// namespace leka
