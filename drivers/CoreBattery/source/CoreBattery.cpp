// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

namespace leka {

float CoreBattery::readVoltage(void)
{
	return _pin.read();
}

}	// namespace leka
