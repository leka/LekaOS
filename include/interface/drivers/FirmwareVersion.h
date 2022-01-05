// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVERS_FIRMWARE_VERSION_H_
#define _LEKA_OS_DRIVERS_FIRMWARE_VERSION_H_

#include <cstdint>

namespace leka {

struct FirmwareVersion {
	uint8_t major;
	uint8_t minor;
	uint16_t revision;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVERS_FIRMWARE_VERSION_H_
