// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

namespace leka {

auto CoreBattery::getVoltage() -> float
{
	auto raw	 = readRawVoltage();
	auto voltage = raw / voltage::divider;
	return voltage;
}

auto CoreBattery::readRawVoltage() -> float
{
	return _pin.read_voltage();
}

}	// namespace leka
