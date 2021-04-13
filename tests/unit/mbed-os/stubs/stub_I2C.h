// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include "I2C.h"
namespace leka {

extern std::vector<char> spy_I2C_read_value;
std::vector<char> spy_temperatureSensor_setValue(std::vector<char> value, int size);
std::vector<char> spy_temperatureSensor_getValue(int size);
}	// namespace leka
