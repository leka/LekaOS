// Leka - LekaOS
// Copyright 2021-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/AnalogIn.h"
#include "drivers/InterruptIn.h"

#include "interface/drivers/Battery.h"

namespace leka {

class CoreBattery : public interface::Battery
{
  public:
	explicit CoreBattery(PinName voltage_pin, mbed::InterruptIn &charge_status_input)
		: _voltage_pin(mbed::AnalogIn(voltage_pin, analog_voltage_reference)), _charge_status_input(charge_status_input)
	{
		// nothing do to
	}

	void onChargeDidStart(std::function<void()> const &callback) final;
	void onChargeDidStop(std::function<void()> const &callback) final;

	auto voltage() -> float final;
	auto level() -> uint8_t final;
	auto isCharging() -> bool final;

	struct Capacity {
		static constexpr auto full			= float {11.50F};
		static constexpr auto three_quarter = float {11.03F};
		static constexpr auto half			= float {10.85F};
		static constexpr auto quarter		= float {10.47F};
		static constexpr auto empty			= float {09.00F};
	};

  private:
	static constexpr auto analog_voltage_reference = float {3.33F};

	struct PolynomialCoefficient {
		static constexpr auto degree_0 = float {47.5F};
		static constexpr auto degree_1 = float {-50.7F};
		static constexpr auto degree_2 = float {15.8F};
	};

	auto readRawVoltage() -> float;
	auto getAverageVoltage() -> float;
	[[nodiscard]] auto convertToRealVoltage(float value) const -> float;

	mbed::AnalogIn _voltage_pin;
	mbed::InterruptIn &_charge_status_input;

	std::function<void()> _on_charge_did_start {};
	std::function<void()> _on_charge_did_stop {};
};

}	// namespace leka
