// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "I2C.h"

namespace leka {

extern char spy_I2C_read_value[64];
char *spy_temperatureSensor_setValue(char *value, int size);
char *spy_temperatureSensor_getValue(const int size);
}	// namespace leka
