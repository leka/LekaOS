// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "CGColor.hpp"
#include "CoreLL.h"
#include "internal/corevideo_config.h"

namespace leka {

struct CGPoint {
	uint32_t x = 0;
	uint32_t y = 0;
};

struct CGPixel {
	explicit CGPixel(CoreLL &ll) : corell(ll) {}

	CGPoint coordinates {0, 0};
	CoreLL &corell;

	void draw(CGColor color)
	{
		uintptr_t destination_address =
			lcd::frame_buffer_address + (4 * (coordinates.y * lcd::dimension::width + coordinates.x));
		uint32_t destinationColor = color.getARGB();

		corell.rawMemoryWrite(destination_address, destinationColor);
	}
};

}	// namespace leka
