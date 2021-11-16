// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

namespace leka {

auto CoreBattery::getVoltage() -> float
{
	auto raw_sum		  = float {};
	const auto iterations = 10;

	for (int i = 0; i < iterations; i++) {
		raw_sum += readRawVoltage();
	}
	auto raw_average = raw_sum / iterations;

	auto voltage = convertToRealVoltage(raw_average);
	return voltage;
}

auto CoreBattery::convertToRealVoltage(float value) const -> float
{
	return PolynomialCoefficient::degree_0 + PolynomialCoefficient::degree_1 * value +
		   PolynomialCoefficient::degree_2 * value * value;
}

auto CoreBattery::readRawVoltage() -> float
{
	return _pin.read_voltage();
}

auto CoreBattery::isInCharge() -> bool
{
	return _charge_input.read() == 1;
}

}	// namespace leka
