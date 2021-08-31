// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CGPIXEL_H_
#define _LEKA_OS_LIB_CGPIXEL_H_

#include <cstdint>

#include "CGColor.h"
#include "LKCoreLL.h"
#include "internal/corevideo_config.h"

namespace leka {

struct Point {
	uint32_t x = 0;
	uint32_t y = 0;
};

struct CGPixel {
	explicit CGPixel(LKCoreLL &ll) : corell(ll) {}

	Point coordinates {0, 0};
	LKCoreLL &corell;

	void draw(CGColor color)
	{
		uintptr_t destination_address =
			lcd::frame_buffer_address + (4 * (coordinates.y * lcd::dimension.width + coordinates.x));
		uint32_t destinationColor = color.getARGB();

		corell.rawMemoryWrite(destination_address, destinationColor);
	}
};

}	// namespace leka

#endif
