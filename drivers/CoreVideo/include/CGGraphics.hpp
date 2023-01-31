// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "CoreLL.h"
#include "internal/corevideo_config.h"

namespace leka {

struct CGColor {
	uint8_t red {};
	uint8_t green {};
	uint8_t blue {};
	uint8_t alpha {0xFF};

	[[nodiscard]] auto getARGB() const -> uint32_t { return alpha << 24 | red << 16 | green << 8 | blue; }

	static const CGColor white;
	static const CGColor black;
	static const CGColor pure_red;
	static const CGColor pure_green;
	static const CGColor pure_blue;
	static const CGColor yellow;
	static const CGColor cyan;
	static const CGColor magenta;
};

constexpr CGColor CGColor::white {0xFF, 0xFF, 0xFF};
constexpr CGColor CGColor::black {0x00, 0x00, 0x00};

constexpr CGColor CGColor::pure_red {0xFF, 0x00, 0x00};
constexpr CGColor CGColor::pure_green {0x00, 0xFF, 0x00};
constexpr CGColor CGColor::pure_blue {0x00, 0x00, 0xFF};

constexpr CGColor CGColor::yellow {0xFF, 0xFF, 0x00};
constexpr CGColor CGColor::cyan {0x00, 0xFF, 0xFF};
constexpr CGColor CGColor::magenta {0xFF, 0x00, 0xFF};

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

struct CGCharacter {
	CGPoint origin {};	 // Top left corner by convention
	uint8_t ascii {};	 // From 0x20 to 0x7F
};

struct CGRectangle {
	CGPoint origin {0, 0};	 // * Top left corner by convention
	uint16_t width {};
	uint16_t height {};
};

}	// namespace leka
