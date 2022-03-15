// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

struct FirmwareVersion {
	uint8_t major;
	uint8_t minor;
	uint16_t revision;	 // ? Use uint16_t instead of uint8_t for compatibility w/ MCUBoot Image format, see
						 // ? https://docs.mcuboot.com/design.html
};

}	// namespace leka
