// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "interface/drivers/STM32Hal.h"

namespace leka {

struct JPEGImageProperties : public JPEG_ConfTypeDef {
	[[nodiscard]] auto getWidthOffset() const -> uint32_t;
};

}	// namespace leka
