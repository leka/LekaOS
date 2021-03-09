// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CGCOLOR_H_
#define _LEKA_OS_LIB_CGCOLOR_H_

#include <stdint.h>

namespace leka {

struct Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha = 0xFF;

	uint32_t getARGB() { return alpha << 24 | red << 16 | green << 8 | blue; }
};

namespace CGColor {

	constexpr Color white {0xFF, 0xFF, 0xFF};
	constexpr Color black {0x00, 0x00, 0x00};

	constexpr Color red {0xFF, 0x00, 0x00};
	constexpr Color green {0x00, 0xFF, 0x00};
	constexpr Color blue {0x00, 0x00, 0xFF};

	constexpr Color yellow {0xFF, 0xFF, 0x00};
	constexpr Color cyan {0x00, 0xFF, 0xFF};
	constexpr Color magenta {0xFF, 0x00, 0xFF};

}	// namespace CGColor

}	// namespace leka

#endif
