// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <cstdint>

#include "RGB.h"

namespace leka {

struct ColorPosition {
	uint8_t position;
	RGB color;
};
}	// namespace leka
