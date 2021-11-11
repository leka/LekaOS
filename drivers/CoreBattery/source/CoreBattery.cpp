// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

namespace leka {

auto CoreBattery::getVoltage() -> float
{
	auto raw			  = 0.f;
	const auto interation = 10;

	for (int i = 0; i < interation; i++) {
		raw += readRawVoltage();
	}
	raw /= interation;

	auto voltage = convertToRealVoltage(raw);
	return voltage;
}

auto CoreBattery::convertToRealVoltage(float value) const -> float
{
	return polynomial::degree_0 + polynomial::degree_1 * value + polynomial::degree_2 * value * value;
}

auto CoreBattery::readRawVoltage() -> float
{
	return _pin.read_voltage();
}

}	// namespace leka
