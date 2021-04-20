// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "I2C.h"
namespace leka {

extern std::vector<char> spy_I2C_read_values;
extern std::vector<char> spy_I2C_write_values;

void spy_temperatureSensor_setValuesBeforeRead(std::vector<char> values);
std::vector<char> spy_temperatureSensor_getValuesAfterWrite();
}	// namespace leka
