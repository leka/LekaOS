// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_LED_BRGB_H_
#define _LEKA_OS_DRIVER_CORE_LED_BRGB_H_

#include <cstdint>

#include "RGB.h"

namespace leka {

struct bRGB {
	uint8_t brightness {};
	RGB color {};

	auto operator<=>(bRGB const &rhs) const -> bool = default;

	static const bRGB white;
	static const bRGB black;
	static const bRGB pure_red;
	static const bRGB pure_green;
	static const bRGB pure_blue;
	static const bRGB yellow;
	static const bRGB cyan;
	static const bRGB magenta;
	static const bRGB off;
};

constexpr bRGB bRGB::white {0xE0 + 16, {0xFF, 0xFF, 0xFF}};
constexpr bRGB bRGB::black {0x00, {0x00, 0x00, 0x00}};

constexpr bRGB bRGB::pure_red {0xE0 + 16, {0xFF, 0x00, 0x00}};
constexpr bRGB bRGB::pure_green {0xE0 + 16, {0x00, 0xFF, 0x00}};
constexpr bRGB bRGB::pure_blue {0xE0 + 16, {0x00, 0x00, 0xFF}};

constexpr bRGB bRGB::yellow {0xE0 + 16, {0xFF, 0xFF, 0x00}};
constexpr bRGB bRGB::cyan {0xE0 + 16, {0x00, 0xFF, 0xFF}};
constexpr bRGB bRGB::magenta {0xE0 + 16, {0xFF, 0x00, 0xFF}};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_LED_BRGB_H_
