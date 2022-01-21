// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

namespace leka {

auto CoreBattery::voltage() -> float
{
	auto raw_average = getAverageVoltage();
	auto voltage	 = convertToRealVoltage(raw_average);

	return voltage;
}

auto CoreBattery::readRawVoltage() -> float
{
	return _voltage_pin.read_voltage();
}

auto CoreBattery::getAverageVoltage() -> float
{
	constexpr auto iterations = int {100};
	auto raw_sum			  = float {};

	for (int i = 0; i < iterations; i++) {
		raw_sum += readRawVoltage();
	}
	return raw_sum / static_cast<float>(iterations);
}

auto CoreBattery::convertToRealVoltage(float value) const -> float
{
	return PolynomialCoefficient::degree_0 + PolynomialCoefficient::degree_1 * value +
		   PolynomialCoefficient::degree_2 * value * value;
}

auto CoreBattery::isCharging() -> bool
{
	return _charge_status_input.read() == 1;
}

}	// namespace leka
