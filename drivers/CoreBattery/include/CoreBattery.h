// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_BATTERY_H_
#define _LEKA_OS_DRIVER_CORE_BATTERY_H_

#include "drivers/AnalogIn.h"
#include "drivers/interfaces/InterfaceDigitalIn.h"

namespace leka {

class CoreBattery
{
  public:
	explicit CoreBattery(PinName voltage_pin, mbed::interface::DigitalIn &charge_status_input)
		: _voltage_pin {mbed::AnalogIn(voltage_pin, analog_voltage_reference)},
		  _charge_status_input(charge_status_input) {};

	auto voltage() -> float;
	auto isCharging() -> bool;

	struct Capacity {
		static constexpr auto full			= float {12.00};
		static constexpr auto three_quarter = float {11.73};
		static constexpr auto half			= float {11.08};
		static constexpr auto quarter		= float {10.47};
		static constexpr auto empty			= float {09.00};
	};

  private:
	static constexpr auto analog_voltage_reference = float {3.33};

	struct PolynomialCoefficient {
		static constexpr auto degree_0 = float {47.5};
		static constexpr auto degree_1 = float {-50.7};
		static constexpr auto degree_2 = float {15.8};
	};

	auto readRawVoltage() -> float;
	auto getAverageVoltage() -> float;
	[[nodiscard]] auto convertToRealVoltage(float value) const -> float;

	mbed::AnalogIn _voltage_pin;
	mbed::interface::DigitalIn &_charge_status_input;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_BATTERY_H_
