// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/AnalogIn.h"

namespace leka {

extern float spy_AnalogIn_value;
extern float spy_AnalogIn_voltage_value;

void spy_AnalogIn_setValue(float value);
void spy_AnalogIn_setVoltageValue(float value);

}	// namespace leka
