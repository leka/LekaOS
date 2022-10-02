// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "drivers/SPI.h"

namespace leka {

extern std::vector<uint8_t> spy_spi_written_values;

auto spy_spi_getValuesAfterWrite() -> std::vector<uint8_t>;

}	// namespace leka
