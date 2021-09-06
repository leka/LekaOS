// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CGCOLOR_H_
#define _LEKA_OS_LIB_CGCOLOR_H_

#include <stdint.h>

namespace leka {

struct CGColor {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha = 0xFF;

	uint32_t getARGB() const { return alpha << 24 | red << 16 | green << 8 | blue; }

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

}	// namespace leka

#endif
