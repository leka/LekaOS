// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreBattery.h"

namespace leka {

float LKCoreBattery::readVoltage(void)
{
	return _pin.read();
}

}	// namespace leka
