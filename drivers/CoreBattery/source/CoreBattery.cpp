// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBattery.h"

#include "platform/Callback.h"

#include "MathUtils.h"

namespace leka {

void CoreBattery::onChargeDidStart(std::function<void()> const &callback)
{
	_on_charge_did_start = callback;
	_charge_status_input.rise(mbed::Callback<void()> {[this] { _on_charge_did_start(); }});
}

void CoreBattery::onChargeDidStop(std::function<void()> const &callback)
{
	_on_charge_did_stop = callback;
	_charge_status_input.fall(mbed::Callback<void()> {[this] { _on_charge_did_stop(); }});
}

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

auto CoreBattery::level() -> uint8_t
{
	using namespace utils::math;

	auto level = uint8_t {0};

	if (auto value = voltage(); value < Capacity::empty) {
		level = 0;
	} else if (value < Capacity::quarter) {
		level = map(value, Capacity::empty, Capacity::quarter, uint8_t {0}, uint8_t {25});
	} else if (value < Capacity::half) {
		level = 25 + map(value, Capacity::quarter, Capacity::half, uint8_t {0}, uint8_t {25});
	} else if (value < Capacity::three_quarter) {
		level = 50 + map(value, Capacity::half, Capacity::three_quarter, uint8_t {0}, uint8_t {25});
	} else if (value < Capacity::full) {
		level = 75 + map(value, Capacity::three_quarter, Capacity::full, uint8_t {0}, uint8_t {25});
	} else {
		level = 100;
	}

	return level;
}

auto CoreBattery::isCharging() -> bool
{
	return _charge_status_input.read() == 1;
}

}	// namespace leka
