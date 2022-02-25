// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

struct FirmwareVersion {
	uint8_t major;
	uint8_t minor;
	uint16_t revision;
};

}	// namespace leka
