// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAccelerometer.h"

namespace leka {

auto CoreAccelerometer::getXYZ() -> std::tuple<mg_t, mg_t, mg_t>
{
	auto data = _lsm6dsox.getData();
	return std::tuple {data.xl.x, data.xl.y, data.xl.z};
}

}	// namespace leka
