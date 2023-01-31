// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "CGPixel.hpp"

namespace leka {

struct Character {
	CGPoint origin {};	 // Top left corner by convention
	uint8_t ascii {};	 // From 0x20 to 0x7F
};

}	// namespace leka
