// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "drivers/I2C.h"

namespace leka {

extern std::vector<uint8_t> spy_I2C_read_values;
extern std::vector<uint8_t> spy_I2C_write_values;

void spy_I2C_setValuesBeforeRead(std::vector<uint8_t> values);
std::vector<uint8_t> spy_I2C_getValuesAfterWrite();
}	// namespace leka
