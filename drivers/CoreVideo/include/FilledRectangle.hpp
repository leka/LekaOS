// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "CGPixel.hpp"

namespace leka {

struct FilledRectangle {
	CGPoint origin {0, 0};	 // * Top left corner by convention
	uint16_t width {};
	uint16_t height {};
};

}	// namespace leka
